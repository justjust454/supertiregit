#include "../OCCTProxy.h"
#include "../OCCTState.h"
#include "../Internal/OCCT.FaceGraphHelpers.inl"

#pragma region Proxy_Face_Build
    /// <summary>
    /// 核心制面引擎：将传入的多个线段 ID 缝合并生成实心面
    /// </summary>
    bool OCCTProxy::CreateFaceFromLines(
        array<int>^ lineIds,
        int newFaceId,
        int r,
        int g,
        int b,
        double geometryTolerance)
    {
        if (myAISContext().IsNull() || lineIds->Length == 0 ||
            geometryTolerance <= Precision::Confusion())
            return false;

        // 1. 收集所有的 Edge
        std::vector<TopoDS_Edge> selectedEdges;
        for (int i = 0; i < lineIds->Length; ++i)
        {
            const int id = lineIds[i];
            const auto shapeIterator = GlobalShapeMap.find(id);
            if (shapeIterator == GlobalShapeMap.end())
                continue;

            Handle(AIS_Shape) aisShape =
                Handle(AIS_Shape)::DownCast(shapeIterator->second);
            if (aisShape.IsNull())
                continue;

            for (TopExp_Explorer explorer(aisShape->Shape(), TopAbs_EDGE);
                explorer.More(); explorer.Next())
            {
                selectedEdges.push_back(TopoDS::Edge(explorer.Current()));
            }
        }

        if (selectedEdges.empty())
            return false;

        Handle(TopTools_HSequenceOfShape) wiresSeq;
        if (!BuildClosedWiresFromIntersectingEdges(
            selectedEdges,
            static_cast<Standard_Real>(geometryTolerance),
            wiresSeq))
        {
            return false;
        }

        // ==========================================================
        // 3. 【绝对严谨的拓扑识别】：通过“真实周长”找出外轮廓
        // ==========================================================
        double maxLength = -1.0;
        int outerIndex = 1;

        for (int i = 1; i <= wiresSeq->Length(); i++) {
            TopoDS_Wire w = TopoDS::Wire(wiresSeq->Value(i));

            if (!w.Closed()) return false; // 用户漏选了线条，未闭合，直接退出

            GProp_GProps props;
            BRepGProp::LinearProperties(w, props);
            double currentLength = props.Mass();

            // 周长最长的，绝对是外侧的主轮廓！
            if (currentLength > maxLength) {
                maxLength = currentLength;
                outerIndex = i;
            }
        }

        TopoDS_Wire outerWire = TopoDS::Wire(wiresSeq->Value(outerIndex));
        BRepBuilderAPI_MakeFace faceMakerOuter(outerWire, Standard_True);
        if (!faceMakerOuter.IsDone()) return false;

        // 此时 finalShape 是一整块实心面
        TopoDS_Shape finalShape = faceMakerOuter.Face();

        // ==========================================================
        // 5. 【核武器级打孔】：把剩下的线圈当做冲压刀具，强行切除！
        // ==========================================================
        for (int i = 1; i <= wiresSeq->Length(); i++) {
            if (i == outerIndex) continue; // 跳过大铁板自己

            TopoDS_Wire holeWire = TopoDS::Wire(wiresSeq->Value(i));

            // 把孔也生成一个实心面
            BRepBuilderAPI_MakeFace faceMakerHole(holeWire, Standard_True);

            if (faceMakerHole.IsDone()) {
                TopoDS_Face holeFace = faceMakerHole.Face();

                // 执行布尔减运算： 大铁板 - 内部孔
                BRepAlgoAPI_Cut cutter(finalShape, holeFace);
                if (cutter.IsDone()) {
                    finalShape = cutter.Shape(); // 更新冲压后的铁板
                }
            }
        }

        if (finalShape.IsNull())
        {
            return false;
        }

        // 防止布尔运算成功，但得到的是不包含面的空结果
        TopExp_Explorer faceCheck(finalShape, TopAbs_FACE);
        if (!faceCheck.More())
        {
            return false;
        }

        // 使用已有的高质量显示函数创建 AIS 对象
        Handle(AIS_Shape) faceAis = MakeHighQualityAISShape(finalShape);
        if (faceAis.IsNull())
        {
            return false;
        }

        // 设置边界显示
        faceAis->Attributes()->SetFaceBoundaryDraw(Standard_True);
        faceAis->Attributes()->SetFaceBoundaryAspect(
            new Prs3d_LineAspect(
                Quantity_NOC_BLACK,
                Aspect_TOL_SOLID,
                2.0));

        // 设置面的颜色和着色模式
        Quantity_Color compColor(
            r / 255.0,
            g / 255.0,
            b / 255.0,
            Quantity_TOC_RGB);

        faceAis->SetDisplayMode(AIS_Shaded);
        faceAis->SetColor(compColor);

        // 防止 ID 冲突时旧对象残留
        auto oldObject = GlobalShapeMap.find(newFaceId);
        if (oldObject != GlobalShapeMap.end())
        {
            if (!oldObject->second.IsNull())
            {
                myAISContext()->Remove(oldObject->second, Standard_False);
            }

            GlobalShapeMap.erase(oldObject);
        }

        // 登记并显示新面
        GlobalShapeMap[newFaceId] = faceAis;
        myAISContext()->Display(faceAis, Standard_False);

        // 清除原边线的选择高亮并刷新
        myAISContext()->ClearSelected(Standard_False);
        myAISContext()->UpdateCurrentViewer();

        if (!myView().IsNull())
        {
            myView()->Redraw();
        }

        return true;
    }
#pragma endregion // Proxy_Face_Build
