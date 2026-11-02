#include "../OCCTProxy.h"
#include "../OCCTState.h"
#include "../Internal/OCCT.CurveHelpers.inl"

#pragma region Proxy_Curve_Edit
    //线条自动连接（帘线专用）
    array<int>^ OCCTProxy::AutoCombineLines(int startId, int newId)
    {
        if (myAISContext().IsNull() || GlobalShapeMap.find(startId) == GlobalShapeMap.end()) 
        {
                return nullptr; 
        } 
            std::vector<int> chainIds; 
            chainIds.push_back(startId);
            bool added = true;
            while (added)
            {
                added = false;
                for (auto const& pair : GlobalShapeMap)
                {
                    int candId = pair.first;
                    if (std::find(chainIds.begin(), chainIds.end(), candId) != chainIds.end()) continue;
                    Handle(AIS_Shape) aisCand = Handle(AIS_Shape)::DownCast(GlobalShapeMap[candId]);
                    if (aisCand.IsNull()) continue;
                    TopoDS_Shape candShape = aisCand->Shape();
                    bool touches = false; 
                    for (int inChainId : chainIds)
                    {
                        Handle(AIS_Shape) aisInChain = Handle(AIS_Shape)::DownCast(GlobalShapeMap[inChainId]);
                        if (aisInChain.IsNull()) continue; TopoDS_Shape inChainShape = aisInChain->Shape();
                        if (AreEdgesTouching(candShape, inChainShape, 1e-2))
                        {
                            touches = true; 
                            break;
                        }
                    }
                    if (touches)
                    {
                        chainIds.push_back(candId);
                        added = true;
                    }
                }
            }
            if (chainIds.size() < 2) return nullptr;
            Handle(TopTools_HSequenceOfShape) edgesSeq = new TopTools_HSequenceOfShape();
            for (int id : chainIds)
            {
                TopoDS_Shape sh = Handle(AIS_Shape)::DownCast(GlobalShapeMap[id])->Shape();
                for (TopExp_Explorer ex(sh, TopAbs_EDGE); ex.More(); ex.Next())
                {
                    edgesSeq->Append(ex.Current());
                }
            }
            Handle(TopTools_HSequenceOfShape) wiresSeq = new TopTools_HSequenceOfShape();
            ShapeAnalysis_FreeBounds::ConnectEdgesToWires(edgesSeq, 1e-2, Standard_False, wiresSeq);
            if (wiresSeq->Length() != 1) return nullptr; // 拓扑太乱存在严重分叉，保险起见拒绝合并 
            TopoDS_Wire rawWire = TopoDS::Wire(wiresSeq->Value(1));
            BRepTools_WireExplorer wireExp(rawWire);
            if (!wireExp.More()) return nullptr;

            TopoDS_Edge currentEdge = wireExp.Current();
            Standard_Real f, l;
            Handle(Geom_Curve) currentCurve = BRep_Tool::Curve(currentEdge, f, l);
            Handle(Geom_TrimmedCurve) trimmedCurve = new Geom_TrimmedCurve(currentCurve, f, l); 
            if (currentEdge.Orientation() == TopAbs_REVERSED) trimmedCurve->Reverse();
            GeomConvert_CompCurveToBSplineCurve concatenator(trimmedCurve);
            wireExp.Next();
            for (; wireExp.More(); wireExp.Next()) 
            { 
                TopoDS_Edge edge = wireExp.Current(); 
                Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, f, l);
                Handle(Geom_TrimmedCurve) tCurve = new Geom_TrimmedCurve(curve, f, l); 
                if (edge.Orientation() == TopAbs_REVERSED) tCurve->Reverse();
                concatenator.Add(tCurve, 1e-2, Standard_True); 
            }
            TopoDS_Edge finalSingleEdge = BRepBuilderAPI_MakeEdge(concatenator.BSplineCurve());
            Handle(AIS_Shape) newAis = new AIS_Shape(finalSingleEdge); 
            newAis->SetColor(Quantity_NOC_RED);
            array<int>^ consumedIds = gcnew array<int>(chainIds.size()); for (size_t i = 0; i < chainIds.size(); i++) 
            {
                int oldId = chainIds[i];
                consumedIds[i] = oldId; 
                myAISContext()->Remove(GlobalShapeMap[oldId], Standard_False); // 从视口抹除 GlobalShapeMap.erase(oldId); // 从档案室销毁 
            }
            GlobalShapeMap[newId] = newAis;
            myAISContext()->Display(newAis, Standard_False); 
            myAISContext()->ClearSelected(Standard_False); 
            myAISContext()->UpdateCurrentViewer(); 
            return consumedIds;

    }













    /// <summary>
    /// 将两条直线合并为一条单一直线 (极点提取法) 国延林 2026.06
    /// </summary>
    /// <param name="id1">第一条线的 ID</param>
    /// <param name="id2">第二条线的 ID</param>
    /// <param name="newId">C# 传来的新线 ID</param>
    /// <returns>合并是否成功</returns>
    bool OCCTProxy::CombineLines(int id1, int id2, int newId)
    {
        if (myAISContext().IsNull()) return false;

        if (GlobalShapeMap.find(id1) == GlobalShapeMap.end() ||
            GlobalShapeMap.find(id2) == GlobalShapeMap.end()) {
            return false;
        }

        Handle(AIS_InteractiveObject) ais1 = GlobalShapeMap[id1];
        Handle(AIS_InteractiveObject) ais2 = GlobalShapeMap[id2];

        TopoDS_Shape shape1 = Handle(AIS_Shape)::DownCast(ais1)->Shape();
        TopoDS_Shape shape2 = Handle(AIS_Shape)::DownCast(ais2)->Shape();

        // 1. 依然借助 MakeWire 来“理顺”这两条线的连接顺序和端点方向
        BRepBuilderAPI_MakeWire wireMaker;
        if (shape1.ShapeType() == TopAbs_EDGE) wireMaker.Add(TopoDS::Edge(shape1));
        else if (shape1.ShapeType() == TopAbs_WIRE) wireMaker.Add(TopoDS::Wire(shape1));

        if (shape2.ShapeType() == TopAbs_EDGE) wireMaker.Add(TopoDS::Edge(shape2));
        else if (shape2.ShapeType() == TopAbs_WIRE) wireMaker.Add(TopoDS::Wire(shape2));

        if (!wireMaker.IsDone()) return false;
        TopoDS_Wire orderedWire = wireMaker.Wire();

        // ==============================================================
        // 2. 【核心黑科技】：降维打击！将 Wire(塑料袋) 融铸为单一 B-Spline 曲线
        // ==============================================================
        BRepTools_WireExplorer wireExp(orderedWire);
        if (!wireExp.More()) return false;

        // a. 提取第一根线，作为“熔炉”的基底
        TopoDS_Edge currentEdge = wireExp.Current();
        Standard_Real f, l;
        Handle(Geom_Curve) currentCurve = BRep_Tool::Curve(currentEdge, f, l);
        Handle(Geom_TrimmedCurve) trimmedCurve = new Geom_TrimmedCurve(currentCurve, f, l);

        // 如果这根线在拓扑中是反向的，我们在数学层面把它反转回来
        if (currentEdge.Orientation() == TopAbs_REVERSED) {
            trimmedCurve->Reverse();
        }

        // 初始化超级样条曲线缝合器
        GeomConvert_CompCurveToBSplineCurve concatenator(trimmedCurve);
        wireExp.Next();

        // b. 遍历剩下的线，在数学底层一段一段地无缝“焊”到 B-Spline 上
        for (; wireExp.More(); wireExp.Next()) {
            TopoDS_Edge edge = wireExp.Current();
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, f, l);
            Handle(Geom_TrimmedCurve) tCurve = new Geom_TrimmedCurve(curve, f, l);

            if (edge.Orientation() == TopAbs_REVERSED) {
                tCurve->Reverse();
            }

            // 0.01 毫米容差，强制合并数学曲率！
            concatenator.Add(tCurve, 1e-2, Standard_True);
        }

        // c. 获取最终的纯数学形态：一条连续无断点的单一曲线！
        Handle(Geom_BSplineCurve) singleBSpline = concatenator.BSplineCurve();

        // d. 将这条单一的数学曲线重新包装成【唯一的一个 Edge】
        TopoDS_Edge finalSingleEdge = BRepBuilderAPI_MakeEdge(singleBSpline);
        // ==============================================================

        // 3. 包装显示并交接户籍 (注意：这次存入档案室的是一个纯粹的 Edge，不再是 Wire 啦！)
        Handle(AIS_Shape) newAis = new AIS_Shape(finalSingleEdge);
        newAis->SetColor(Quantity_NOC_RED);

        myAISContext()->Remove(ais1, Standard_False);
        myAISContext()->Remove(ais2, Standard_False);
        GlobalShapeMap.erase(id1);
        GlobalShapeMap.erase(id2);

        GlobalShapeMap[newId] = newAis;
        myAISContext()->Display(newAis, Standard_False);

        myAISContext()->ClearSelected(Standard_False);
        myAISContext()->UpdateCurrentViewer();

        return true;
    }
   
    /// <summary>
    /// 线打断
    /// </summary>
    /// <param name="targetId">要被打断的线 (Argument)</param>
    /// <param name="toolId">作为切刀的相交线 (Tool)</param>
    /// <param name="startingNewId">由 C# 传入的可用起始 ID</param>
    /// <returns>返回打断后生成的所有新散线 ID 数组。如果没相交/失败，返回 nullptr</returns>
    array<int>^ OCCTProxy::SplitLine(
        int targetId,
        int toolId,
        int startingNewId,
        double geometryTolerance)
    {
        if (myAISContext().IsNull() || targetId == toolId ||
            geometryTolerance <= Precision::Confusion())
            return nullptr;

        const auto targetIterator = GlobalShapeMap.find(targetId);
        const auto toolIterator = GlobalShapeMap.find(toolId);
        if (targetIterator == GlobalShapeMap.end() ||
            toolIterator == GlobalShapeMap.end())
            return nullptr;

        Handle(AIS_Shape) targetAis =
            Handle(AIS_Shape)::DownCast(targetIterator->second);
        Handle(AIS_Shape) toolAis =
            Handle(AIS_Shape)::DownCast(toolIterator->second);
        if (targetAis.IsNull() || toolAis.IsNull())
            return nullptr;

        const TopoDS_Shape targetShape = targetAis->Shape();
        const TopoDS_Shape toolShape = toolAis->Shape();
        if (targetShape.IsNull() || toolShape.IsNull())
            return nullptr;

        std::vector<TopoDS_Edge> targetEdges;
        int originalEdgeCount = 0;
        bool usedEndpointSnap = false;
        SnapSplitPlan snapPlan;
        try
        {
            TopTools_ListOfShape arguments;
            arguments.Append(targetShape);
            TopTools_ListOfShape tools;
            tools.Append(toolShape);

            BRepAlgoAPI_Splitter splitter;
            splitter.SetArguments(arguments);
            splitter.SetTools(tools);
            splitter.Build();

            if (splitter.IsDone())
            {
                for (TopExp_Explorer explorer(targetShape, TopAbs_EDGE);
                    explorer.More(); explorer.Next())
                {
                    ++originalEdgeCount;
                    const TopoDS_Edge originalEdge = TopoDS::Edge(explorer.Current());
                    const TopTools_ListOfShape& modified = splitter.Modified(originalEdge);

                    if (modified.IsEmpty())
                    {
                        targetEdges.push_back(originalEdge);
                        continue;
                    }

                    for (TopTools_ListIteratorOfListOfShape item(modified);
                        item.More(); item.Next())
                    {
                        for (TopExp_Explorer edgeExplorer(item.Value(), TopAbs_EDGE);
                            edgeExplorer.More(); edgeExplorer.Next())
                        {
                            targetEdges.push_back(TopoDS::Edge(edgeExplorer.Current()));
                        }
                    }
                }
            }
        }
        catch (const Standard_Failure&)
        {
            targetEdges.clear();
        }
        if (originalEdgeCount == 0 ||
            targetEdges.size() <= static_cast<std::size_t>(originalEdgeCount))
        {
            targetEdges.clear();
            if (!BuildSnapSplitPlan(
                targetShape, toolShape,
                static_cast<Standard_Real>(geometryTolerance), snapPlan))
                return nullptr;

            targetEdges = snapPlan.TargetEdges;
            usedEndpointSnap = true;
        }

        if (targetEdges.empty())
            return nullptr;
        std::vector<Handle(AIS_Shape)> newTargetObjects;
        newTargetObjects.reserve(targetEdges.size());
        for (std::size_t i = 0; i < targetEdges.size(); ++i)
        {
            const int newId = startingNewId + static_cast<int>(i);
            if (GlobalShapeMap.find(newId) != GlobalShapeMap.end())
                return nullptr;

            Handle(AIS_Shape) newAis = new AIS_Shape(targetEdges[i]);
            if (newAis.IsNull())
                return nullptr;
            newAis->SetColor(Quantity_NOC_RED);
            if (targetAis->HasWidth())
                newAis->SetWidth(targetAis->Width());
            newTargetObjects.push_back(newAis);
        }

        Handle(AIS_Shape) snappedToolAis;
        if (usedEndpointSnap)
        {
            snappedToolAis = new AIS_Shape(snapPlan.SnappedToolEdge);
            if (snappedToolAis.IsNull())
                return nullptr;

            if (toolAis->HasColor())
            {
                Quantity_Color toolColor;
                toolAis->Color(toolColor);
                snappedToolAis->SetColor(toolColor);
            }
            if (toolAis->HasWidth())
                snappedToolAis->SetWidth(toolAis->Width());
        }

        array<int>^ newIds =
            gcnew array<int>(static_cast<int>(targetEdges.size()));

        myAISContext()->Remove(targetIterator->second, Standard_False);
        GlobalShapeMap.erase(targetId);
        if (usedEndpointSnap)
        {
            myAISContext()->Remove(toolIterator->second, Standard_False);
            GlobalShapeMap[toolId] = snappedToolAis;
            myAISContext()->Display(snappedToolAis, Standard_False);
        }

        for (std::size_t i = 0; i < newTargetObjects.size(); ++i)
        {
            const int newId = startingNewId + static_cast<int>(i);
            GlobalShapeMap[newId] = newTargetObjects[i];
            myAISContext()->Display(newTargetObjects[i], Standard_False);
            newIds[static_cast<int>(i)] = newId;
        }

        myAISContext()->ClearSelected(Standard_False);
        myAISContext()->UpdateCurrentViewer();
        return newIds;
    }
    /// <summary>
    /// 批量删除当前视口中所有被选中的线条，并将死掉的 ID 汇报给 C# 前端
    /// </summary>
    bool OCCTProxy::DeleteLine(int id)
    {
        if (myAISContext().IsNull()) return false;

        // 在档案室中查找这个 ID
        if (GlobalShapeMap.find(id) == GlobalShapeMap.end()) {
            return false;
        }

        // 1. 从 3D 视口中彻底移除显示
        myAISContext()->Remove(GlobalShapeMap[id], Standard_False);

        // 2. 从底层的 C++ 档案室字典中销毁记录
        GlobalShapeMap.erase(id);

        // 3. 清除残留的选择高亮状态，并刷新屏幕
        myAISContext()->ClearSelected(Standard_False);
        myAISContext()->UpdateCurrentViewer();

        return true;
    }
#pragma endregion // Proxy_Curve_Edit
