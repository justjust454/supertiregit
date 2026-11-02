#include "../OCCTProxy.h"
#include "../OCCTState.h"
#include "../Internal/OCCT.TopologyHelpers.inl"

#pragma region Proxy_Topology_Edit
    // 拓扑编辑：清理红、绿、蓝临时边
// ============================================================
    void OCCTProxy::ClearTopologyEdgeDisplays()
    {
        if (myAISContext().IsNull())
            return;

        for (std::size_t i = 0;
            i < GlobalTopologyEdgeRecords.size();
            ++i)
        {
            Handle(AIS_Shape) displayObject =
                GlobalTopologyEdgeRecords[i].DisplayObject;

            if (!displayObject.IsNull())
            {
                myAISContext()->Remove(
                    displayObject,
                    Standard_False);
            }
        }

        GlobalTopologyEdgeRecords.clear();
    }

    // 只关闭普通模型对象的选择模式。
    // 不影响 AIS_ViewCube 等视图辅助对象。
    void OCCTProxy::DeactivateNormalShapeSelectionOnly()
    {
        if (myAISContext().IsNull())
            return;

        for (auto iterator = GlobalShapeMap.begin();
            iterator != GlobalShapeMap.end();
            ++iterator)
        {
            Handle(AIS_InteractiveObject) object =
                iterator->second;

            if (object.IsNull())
                continue;

            // 只关闭当前模型对象，不关闭整个交互上下文
            myAISContext()->Deactivate(object);
        }
    }
    
    
    
    // 恢复普通几何对象的整体选择模式
    void OCCTProxy::RestoreNormalShapeSelection()
    {
        if (myAISContext().IsNull())
            return;

        DeactivateNormalShapeSelectionOnly();

        for (auto iterator = GlobalShapeMap.begin();
            iterator != GlobalShapeMap.end();
            ++iterator)
        {
            Handle(AIS_InteractiveObject) object =
                iterator->second;

            if (object.IsNull())
                continue;

            if (myAISContext()->IsDisplayed(object))
            {
                myAISContext()->Activate(
                    object,
                    0,
                    Standard_False);
            }
        }
    }


    // 重新分析当前 Component 的自由边、共享边和抑制边
    bool OCCTProxy::RebuildTopologyEdgeDisplays(
        double tolerance)
    {
        if (myAISContext().IsNull())
            return false;

        ClearTopologyEdgeDisplays();
        DeactivateNormalShapeSelectionOnly();

        BRep_Builder compoundBuilder;
        TopoDS_Compound componentCompound;
        compoundBuilder.MakeCompound(componentCompound);

        Standard_Integer validShapeCount = 0;

        for (std::size_t i = 0;
            i < GlobalTopologyComponentIds.size();
            ++i)
        {
            const int shapeId =
                GlobalTopologyComponentIds[i];

            auto shapeIterator =
                GlobalShapeMap.find(shapeId);

            if (shapeIterator == GlobalShapeMap.end())
                continue;

            Handle(AIS_Shape) aisShape =
                Handle(AIS_Shape)::DownCast(
                    shapeIterator->second);

            if (aisShape.IsNull() ||
                aisShape->Shape().IsNull())
            {
                continue;
            }

            compoundBuilder.Add(
                componentCompound,
                aisShape->Shape());

            ++validShapeCount;
        }

        if (validShapeCount == 0)
            return false;

        TopTools_IndexedDataMapOfShapeListOfShape edgeFaceMap;

        TopExp::MapShapesAndAncestors(
            componentCompound,
            TopAbs_EDGE,
            TopAbs_FACE,
            edgeFaceMap);

        for (Standard_Integer edgeIndex = 1;
            edgeIndex <= edgeFaceMap.Extent();
            ++edgeIndex)
        {
            const TopoDS_Edge edge =
                TopoDS::Edge(edgeFaceMap.FindKey(edgeIndex));

            const TopTools_ListOfShape& ancestorFaces =
                edgeFaceMap.FindFromIndex(edgeIndex);

            // 不属于面的普通构造线不作为拓扑边显示
            if (ancestorFaces.Extent() == 0)
                continue;

            TopologyEdgeDisplayRecord record;
            record.Edge = edge;
            record.FaceUseCount = ancestorFaces.Extent();
            // 查找这条边属于哪个 GlobalShapeMap 顶层 Shape
            for (std::size_t ownerIndex = 0;
                ownerIndex < GlobalTopologyComponentIds.size();
                ++ownerIndex)
            {
                const int ownerId =
                    GlobalTopologyComponentIds[ownerIndex];

                auto ownerIterator =
                    GlobalShapeMap.find(ownerId);

                if (ownerIterator == GlobalShapeMap.end())
                    continue;

                Handle(AIS_Shape) ownerAIS =
                    Handle(AIS_Shape)::DownCast(
                        ownerIterator->second);

                if (ownerAIS.IsNull())
                    continue;

                if (ShapeContainsTopologyEdge(
                    ownerAIS->Shape(),
                    edge))
                {
                    record.OwnerShapeIds.push_back(ownerId);
                }
            }

            if (record.OwnerShapeIds.empty())
                continue;

            Quantity_Color displayColor(
                1.0,
                0.0,
                0.0,
                Quantity_TOC_RGB);

            // 绿色：共享边
            if (record.FaceUseCount >= 2)
            {
                displayColor = Quantity_Color(
                    0.0,
                    1.0,
                    0.0,
                    Quantity_TOC_RGB);
            }

            record.DisplayObject =
                new AIS_Shape(edge);

            record.DisplayObject->SetColor(displayColor);
            record.DisplayObject->SetWidth(3.0);

            myAISContext()->Display(
                record.DisplayObject,
                Standard_False);

            // 临时 Edge 采用整体选择模式
            myAISContext()->Activate(
                record.DisplayObject,
                0,
                Standard_False);

            GlobalTopologyEdgeRecords.push_back(record);
        }

        myAISContext()->ClearSelected(Standard_False);
        myAISContext()->UpdateCurrentViewer();

        return !GlobalTopologyEdgeRecords.empty();
    }


    // 进入拓扑编辑模式
    bool OCCTProxy::BeginTopologyEdit(
        array<int>^ componentShapeIds,
        double tolerance)
    {
        if (myAISContext().IsNull() ||
            componentShapeIds == nullptr ||
            componentShapeIds->Length == 0)
        {
            return false;
        }

        // 防止重复进入
        FinishTopologyEdit();

        GlobalTopologyComponentIds.clear();
        GlobalTopologyBackupObjects.clear();
        GlobalTopologyBackupDisplayed.clear();

        for each (int shapeId in componentShapeIds)
        {
            auto iterator = GlobalShapeMap.find(shapeId);

            if (iterator == GlobalShapeMap.end() ||
                iterator->second.IsNull())
            {
                continue;
            }

            GlobalTopologyComponentIds.push_back(shapeId);

            // 保存原始 AIS 对象，用于取消时恢复
            GlobalTopologyBackupObjects[shapeId] =
                iterator->second;

            GlobalTopologyBackupDisplayed[shapeId] =
                myAISContext()->IsDisplayed(iterator->second);
        }

        if (GlobalTopologyComponentIds.empty())
            return false;

        GlobalTopologyEditActive = Standard_True;

        if (!RebuildTopologyEdgeDisplays(tolerance))
        {
            FinishTopologyEdit();
            return false;
        }

        return true;
    }


    // 返回值：
    // [0] 状态码
    // [1] 保留的 Shape ID
    // [2] 被合并删除的 Shape ID
    array<int>^ OCCTProxy::MakeTopologyResult(
        int status,
        int keptShapeId,
        int removedShapeId)
    {
        array<int>^ result = gcnew array<int>(3);

        result[0] = status;
        result[1] = keptShapeId;
        result[2] = removedShapeId;

        return result;
    }


    // 点击一条红色自由边后自动寻找匹配边并缝合
    array<int>^ OCCTProxy::SewSelectedTopologyEdge(
        double tolerance)
    {
        if (!GlobalTopologyEditActive ||
            myAISContext().IsNull())
        {
            return MakeTopologyResult(-4, -1, -1);
        }

        if (tolerance <= Precision::Confusion())
            tolerance = 0.01;

        Handle(AIS_InteractiveObject) selectedObject;

        for (myAISContext()->InitSelected();
            myAISContext()->MoreSelected();
            myAISContext()->NextSelected())
        {
            selectedObject =
                myAISContext()->SelectedInteractive();

            if (!selectedObject.IsNull())
                break;
        }

        if (selectedObject.IsNull())
            return MakeTopologyResult(0, -1, -1);

        int selectedRecordIndex = -1;

        for (std::size_t i = 0;
            i < GlobalTopologyEdgeRecords.size();
            ++i)
        {
            if (GlobalTopologyEdgeRecords[i].DisplayObject ==
                selectedObject)
            {
                selectedRecordIndex =
                    static_cast<int>(i);
                break;
            }
        }

        if (selectedRecordIndex < 0)
            return MakeTopologyResult(0, -1, -1);

        TopologyEdgeDisplayRecord selectedRecord =
            GlobalTopologyEdgeRecords[selectedRecordIndex];

        // 绿色边已经是共享边，不需要重复缝合。
        if (selectedRecord.FaceUseCount >= 2)
        {
            myAISContext()->ClearSelected(Standard_False);
            myAISContext()->UpdateCurrentViewer();
            return MakeTopologyResult(2, -1, -1);
        }

        // ========================================================
        // 红色自由边：自动搜索与它重合的另一条自由边。
        // ========================================================
        int bestCandidateIndex = -1;

        Standard_Real bestScore =
            std::numeric_limits<Standard_Real>::max();

        Standard_Real secondBestScore =
            std::numeric_limits<Standard_Real>::max();

        for (std::size_t candidateIndex = 0;
            candidateIndex < GlobalTopologyEdgeRecords.size();
            ++candidateIndex)
        {
            if (static_cast<int>(candidateIndex) ==
                selectedRecordIndex)
            {
                continue;
            }

            const TopologyEdgeDisplayRecord& candidateRecord =
                GlobalTopologyEdgeRecords[candidateIndex];

            // 只能把两条自由边缝合
            if (candidateRecord.FaceUseCount != 1)
            {
                continue;
            }

            Standard_Real candidateScore = 0.0;

            if (!TopologyEdgesGeometricallyMatch(
                selectedRecord.Edge,
                candidateRecord.Edge,
                tolerance,
                candidateScore))
            {
                continue;
            }

            if (candidateScore < bestScore)
            {
                secondBestScore = bestScore;
                bestScore = candidateScore;
                bestCandidateIndex =
                    static_cast<int>(candidateIndex);
            }
            else if (candidateScore < secondBestScore)
            {
                secondBestScore = candidateScore;
            }
        }

        if (bestCandidateIndex < 0)
        {
            myAISContext()->ClearSelected(Standard_False);
            return MakeTopologyResult(-1, -1, -1);
        }

        // 同时存在两个几乎完全相同的候选边时拒绝自动处理
        if (secondBestScore <
            std::numeric_limits<Standard_Real>::max() &&
            secondBestScore <= bestScore + tolerance * 0.25)
        {
            myAISContext()->ClearSelected(Standard_False);
            return MakeTopologyResult(-2, -1, -1);
        }

        TopologyEdgeDisplayRecord candidateRecord =
            GlobalTopologyEdgeRecords[bestCandidateIndex];

        if (selectedRecord.OwnerShapeIds.empty() ||
            candidateRecord.OwnerShapeIds.empty())
        {
            return MakeTopologyResult(-4, -1, -1);
        }

        const int firstOwnerId =
            selectedRecord.OwnerShapeIds[0];

        const int secondOwnerId =
            candidateRecord.OwnerShapeIds[0];

        auto firstOwnerIterator =
            GlobalShapeMap.find(firstOwnerId);

        auto secondOwnerIterator =
            GlobalShapeMap.find(secondOwnerId);

        if (firstOwnerIterator == GlobalShapeMap.end() ||
            secondOwnerIterator == GlobalShapeMap.end())
        {
            return MakeTopologyResult(-4, -1, -1);
        }

        Handle(AIS_Shape) firstOwnerAIS =
            Handle(AIS_Shape)::DownCast(
                firstOwnerIterator->second);

        Handle(AIS_Shape) secondOwnerAIS =
            Handle(AIS_Shape)::DownCast(
                secondOwnerIterator->second);

        if (firstOwnerAIS.IsNull() ||
            secondOwnerAIS.IsNull())
        {
            return MakeTopologyResult(-4, -1, -1);
        }

        try
        {
            BRepBuilderAPI_Sewing sewing(
                tolerance,
                Standard_True,
                Standard_True,
                Standard_True,
                Standard_False);

            sewing.Add(firstOwnerAIS->Shape());

            if (firstOwnerId != secondOwnerId)
                sewing.Add(secondOwnerAIS->Shape());

            sewing.Perform();

            TopoDS_Shape sewedShape =
                sewing.SewedShape();

            if (sewedShape.IsNull())
                return MakeTopologyResult(-3, -1, -1);

            // 必须确认缝合结果中确实出现了共享边
            TopoDS_Edge newSharedEdge;

            if (!FindMatchingSharedEdge(
                sewedShape,
                selectedRecord.Edge,
                tolerance,
                newSharedEdge))
            {
                return MakeTopologyResult(-3, -1, -1);
            }

            // 保存原对象的显示颜色
            Quantity_Color originalColor;
            Standard_Boolean hasOriginalColor =
                firstOwnerAIS->HasColor();

            if (hasOriginalColor)
                firstOwnerAIS->Color(originalColor);

            Standard_Boolean shouldDisplay =
                myAISContext()->IsDisplayed(firstOwnerAIS) ||
                myAISContext()->IsDisplayed(secondOwnerAIS);

            ClearTopologyEdgeDisplays();
            myAISContext()->ClearSelected(Standard_False);

            myAISContext()->Remove(
                firstOwnerAIS,
                Standard_False);

            if (firstOwnerId != secondOwnerId)
            {
                myAISContext()->Remove(
                    secondOwnerAIS,
                    Standard_False);
            }

            Handle(AIS_Shape) sewedAIS =
                MakeHighQualityAISShape(sewedShape);

            sewedAIS->SetDisplayMode(AIS_Shaded);

            if (hasOriginalColor)
                sewedAIS->SetColor(originalColor);

            // 保留第一条边所属 Shape 的 ID
            GlobalShapeMap[firstOwnerId] = sewedAIS;

            int removedShapeId = -1;

            if (firstOwnerId != secondOwnerId)
            {
                GlobalShapeMap.erase(secondOwnerId);
                removedShapeId = secondOwnerId;
            }

            if (shouldDisplay)
            {
                myAISContext()->Display(
                    sewedAIS,
                    Standard_False);
            }

            RebuildTopologyEdgeDisplays(tolerance);

            return MakeTopologyResult(
                1,
                firstOwnerId,
                removedShapeId);
        }
        catch (const Standard_Failure&)
        {
            return MakeTopologyResult(-3, -1, -1);
        }
    }


    // 完成：保留缝合结果，只清理临时拓扑边
    void OCCTProxy::FinishTopologyEdit()
    {
        if (myAISContext().IsNull())
            return;

        ClearTopologyEdgeDisplays();
        RestoreNormalShapeSelection();

        GlobalTopologyComponentIds.clear();
        GlobalTopologyBackupObjects.clear();
        GlobalTopologyBackupDisplayed.clear();
        GlobalTopologyEditActive = Standard_False;

        myAISContext()->ClearSelected(Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }


    // 取消：恢复进入拓扑模式前的 Component
    void OCCTProxy::CancelTopologyEdit()
    {
        if (myAISContext().IsNull() ||
            !GlobalTopologyEditActive)
        {
            return;
        }

        ClearTopologyEdgeDisplays();
        DeactivateNormalShapeSelectionOnly();

        // 删除当前编辑后的对象
        for (std::size_t i = 0;
            i < GlobalTopologyComponentIds.size();
            ++i)
        {
            const int shapeId =
                GlobalTopologyComponentIds[i];

            auto currentIterator =
                GlobalShapeMap.find(shapeId);

            if (currentIterator == GlobalShapeMap.end())
                continue;

            if (!currentIterator->second.IsNull())
            {
                myAISContext()->Remove(
                    currentIterator->second,
                    Standard_False);
            }

            GlobalShapeMap.erase(currentIterator);
        }

        // 恢复进入拓扑编辑前的对象
        for (auto backupIterator =
            GlobalTopologyBackupObjects.begin();
            backupIterator != GlobalTopologyBackupObjects.end();
            ++backupIterator)
        {
            const int shapeId =
                backupIterator->first;

            Handle(AIS_InteractiveObject) originalObject =
                backupIterator->second;

            GlobalShapeMap[shapeId] = originalObject;

            if (!originalObject.IsNull() &&
                GlobalTopologyBackupDisplayed[shapeId])
            {
                myAISContext()->Display(
                    originalObject,
                    Standard_False);
            }
        }

        RestoreNormalShapeSelection();

        GlobalTopologyComponentIds.clear();
        GlobalTopologyBackupObjects.clear();
        GlobalTopologyBackupDisplayed.clear();
        GlobalTopologyEditActive = Standard_False;

        myAISContext()->ClearSelected(Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }

#pragma endregion // Proxy_Topology_Edit
