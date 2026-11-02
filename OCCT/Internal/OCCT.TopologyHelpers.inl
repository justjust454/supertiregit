static bool ShapeContainsTopologyEdge(
    const TopoDS_Shape& shape,
    const TopoDS_Edge& targetEdge)
{
    if (shape.IsNull() || targetEdge.IsNull())
        return false;

    for (TopExp_Explorer explorer(shape, TopAbs_EDGE);
        explorer.More();
        explorer.Next())
    {
        if (explorer.Current().IsSame(targetEdge))
            return true;
    }

    return false;
}

// 计算一个空间点到一条 Edge 的最小距离
static bool DistanceFromPointToTopologyEdge(
    const gp_Pnt& point,
    const TopoDS_Edge& edge,
    Standard_Real& distance)
{
    if (edge.IsNull())
        return false;

    try
    {
        TopoDS_Vertex pointVertex =
            BRepBuilderAPI_MakeVertex(point);

        BRepExtrema_DistShapeShape distanceTool(
            pointVertex,
            edge);

        if (!distanceTool.IsDone())
            distanceTool.Perform();

        if (!distanceTool.IsDone() ||
            distanceTool.NbSolution() == 0)
        {
            return false;
        }

        distance = distanceTool.Value();
        return true;
    }
    catch (const Standard_Failure&)
    {
        return false;
    }
}

// 从 sourceEdge 上取多个采样点，检查这些点到 targetEdge 的最大距离
static bool SampleTopologyEdgeDistance(
    const TopoDS_Edge& sourceEdge,
    const TopoDS_Edge& targetEdge,
    const Standard_Integer sampleCount,
    Standard_Real& maximumDistance)
{
    maximumDistance = 0.0;

    try
    {
        BRepAdaptor_Curve sourceCurve(sourceEdge);

        const Standard_Real firstParameter =
            sourceCurve.FirstParameter();

        const Standard_Real lastParameter =
            sourceCurve.LastParameter();

        if (sampleCount < 2)
            return false;

        for (Standard_Integer i = 0;
            i < sampleCount;
            ++i)
        {
            const Standard_Real ratio =
                static_cast<Standard_Real>(i) /
                static_cast<Standard_Real>(sampleCount - 1);

            const Standard_Real parameter =
                firstParameter +
                (lastParameter - firstParameter) * ratio;

            const gp_Pnt samplePoint =
                sourceCurve.Value(parameter);

            Standard_Real currentDistance = 0.0;

            if (!DistanceFromPointToTopologyEdge(
                samplePoint,
                targetEdge,
                currentDistance))
            {
                return false;
            }

            maximumDistance =
                std::max(maximumDistance, currentDistance);
        }

        return true;
    }
    catch (const Standard_Failure&)
    {
        return false;
    }
}

// 判断两条自由边是否属于同一条几何边界
static bool TopologyEdgesGeometricallyMatch(
    const TopoDS_Edge& firstEdge,
    const TopoDS_Edge& secondEdge,
    const Standard_Real tolerance,
    Standard_Real& matchingScore)
{
    matchingScore = std::numeric_limits<Standard_Real>::max();

    if (firstEdge.IsNull() ||
        secondEdge.IsNull() ||
        tolerance <= Precision::Confusion())
    {
        return false;
    }

    // 已经是同一个拓扑 Edge，不需要再次匹配
    if (firstEdge.IsSame(secondEdge))
        return false;

    try
    {
        BRepAdaptor_Curve firstCurve(firstEdge);
        BRepAdaptor_Curve secondCurve(secondEdge);

        const Standard_Real firstLength =
            GCPnts_AbscissaPoint::Length(
                firstCurve,
                firstCurve.FirstParameter(),
                firstCurve.LastParameter());

        const Standard_Real secondLength =
            GCPnts_AbscissaPoint::Length(
                secondCurve,
                secondCurve.FirstParameter(),
                secondCurve.LastParameter());

        if (firstLength <= Precision::Confusion() ||
            secondLength <= Precision::Confusion())
        {
            return false;
        }

        const Standard_Real maximumLength =
            std::max(firstLength, secondLength);

        const Standard_Real lengthDifference =
            std::abs(firstLength - secondLength);

        // 防止把只有一个交点、但长度明显不同的边误认为匹配边
        const Standard_Real allowedLengthDifference =
            std::max(
                tolerance * 4.0,
                maximumLength * 0.02);

        if (lengthDifference > allowedLengthDifference)
            return false;

        Standard_Real firstToSecondDistance = 0.0;
        Standard_Real secondToFirstDistance = 0.0;

        // 双向采样，防止短边只贴在长边的一部分上
        if (!SampleTopologyEdgeDistance(
            firstEdge,
            secondEdge,
            11,
            firstToSecondDistance))
        {
            return false;
        }

        if (!SampleTopologyEdgeDistance(
            secondEdge,
            firstEdge,
            11,
            secondToFirstDistance))
        {
            return false;
        }

        const Standard_Real maximumDistance =
            std::max(
                firstToSecondDistance,
                secondToFirstDistance);

        if (maximumDistance > tolerance)
            return false;

        matchingScore =
            maximumDistance + lengthDifference;

        return true;
    }
    catch (const Standard_Failure&)
    {
        return false;
    }
}

// 从缝合结果中寻找与原自由边对应的新共享边
// 从缝合结果中寻找与原自由边对应的新共享边
static bool FindMatchingSharedEdge(
    const TopoDS_Shape& shape,
    const TopoDS_Edge& referenceEdge,
    const Standard_Real tolerance,
    TopoDS_Edge& sharedEdge)
{
    if (shape.IsNull() || referenceEdge.IsNull())
        return false;

    TopTools_IndexedDataMapOfShapeListOfShape edgeFaceMap;

    TopExp::MapShapesAndAncestors(
        shape,
        TopAbs_EDGE,
        TopAbs_FACE,
        edgeFaceMap);

    Standard_Real bestScore =
        std::numeric_limits<Standard_Real>::max();

    bool found = false;

    for (Standard_Integer index = 1;
        index <= edgeFaceMap.Extent();
        ++index)
    {
        const TopTools_ListOfShape& faces =
            edgeFaceMap.FindFromIndex(index);

        // 只有同时属于两个面的边才是共享边
        if (faces.Extent() < 2)
            continue;

        const TopoDS_Edge candidateEdge =
            TopoDS::Edge(edgeFaceMap.FindKey(index));

        Standard_Real score = 0.0;

        if (!TopologyEdgesGeometricallyMatch(
            referenceEdge,
            candidateEdge,
            tolerance,
            score))
        {
            // Sewing 有时会复用原 Edge
            if (!referenceEdge.IsSame(candidateEdge))
                continue;

            score = 0.0;
        }

        if (score < bestScore)
        {
            bestScore = score;
            sharedEdge = candidateEdge;
            found = true;
        }
    }

    return found;
}

