// 面边界线网中的“节点”：容差范围内重合的端点会被视为同一个节点。
struct FaceGraphNode
{
    // 节点坐标，用于判断两个端点是否在容差范围内重合。
    gp_Pnt Point;
    // 与节点相连的分段边编号，用于计算节点的度。
    std::vector<int> IncidentEdges;
};

// 记录一条分段边首、尾两端连接的图节点编号。
struct FaceGraphEdgeNodes
{
    int First;
    int Last;
};

// 在容差范围内查找已有节点；找不到时创建新节点。
// 作用是合并数值上没有完全重合、但几何上应当连接的端点。
static int FindOrAddFaceGraphNode(
    const gp_Pnt& point,
    const Standard_Real tolerance,
    std::vector<FaceGraphNode>& nodes)
{
    // 检查已有节点，避免同一个几何交点被重复登记。
    for (std::size_t i = 0; i < nodes.size(); ++i)
    {
        if (nodes[i].Point.Distance(point) <= tolerance)
            return static_cast<int>(i);
    }

    // 没找到可合并节点，为当前端点建立新节点。
    FaceGraphNode node;
    node.Point = point;
    nodes.push_back(node);
    return static_cast<int>(nodes.size() - 1);
}

// 读取 OCC Edge 的首尾顶点，并转换为线网节点编号。
static bool GetFaceGraphEdgeNodes(
    const TopoDS_Edge& edge,
    const Standard_Real tolerance,
    std::vector<FaceGraphNode>& nodes,
    FaceGraphEdgeNodes& edgeNodes)
{
    TopoDS_Vertex firstVertex;
    TopoDS_Vertex lastVertex;
    // 取得考虑 Edge 方向后的首、尾顶点。
    TopExp::Vertices(edge, firstVertex, lastVertex, Standard_True);

    if (firstVertex.IsNull() || lastVertex.IsNull())
        return false;

    const gp_Pnt firstPoint = BRep_Tool::Pnt(firstVertex);
    const gp_Pnt lastPoint = BRep_Tool::Pnt(lastVertex);

    edgeNodes.First = FindOrAddFaceGraphNode(firstPoint, tolerance, nodes);
    edgeNodes.Last = FindOrAddFaceGraphNode(lastPoint, tolerance, nodes);
    return true;
}

// 从互相穿过、并可能带有多余伸出部分的共面边线中提取闭合轮廓。
// 处理顺序：交点分段 -> 建立端点拓扑图 -> 递归删除悬空边 -> 连接闭合 Wire。
static bool BuildClosedWiresFromIntersectingEdges(
    const std::vector<TopoDS_Edge>& inputEdges,
    const Standard_Real tolerance,
    Handle(TopTools_HSequenceOfShape)& wires)
{
    // 输入为空或容差小于 OCC 最小精度时，不能进行拓扑整理。
    if (inputEdges.empty() || tolerance <= Precision::Confusion())
        return false;

    try
    {
        // 第 1 步：收集用户选中的全部边。
        TopTools_ListOfShape arguments;
        for (std::size_t i = 0; i < inputEdges.size(); ++i)
            arguments.Append(inputEdges[i]);

        // General Fuse 计算所有边的交点，并在交点处自动切成小段。
        // 非破坏模式保证这里只生成临时分段，不修改视口中的原始线条。
        BOPAlgo_Builder splitter;
        splitter.SetArguments(arguments);
        splitter.SetNonDestructive(Standard_True);
        splitter.SetFuzzyValue(tolerance);
        splitter.Perform();

        // 求交失败或没有有效结果时，终止制面。
        if (splitter.HasErrors() || splitter.Shape().IsNull())
            return false;

        // 第 2 步：收集求交后的分段边，并去除重复拓扑边。
        TopTools_IndexedMapOfShape uniqueEdgeMap;
        for (TopExp_Explorer explorer(splitter.Shape(), TopAbs_EDGE);
            explorer.More(); explorer.Next())
        {
            uniqueEdgeMap.Add(explorer.Current());
        }

        if (uniqueEdgeMap.Extent() == 0)
            return false;

        std::vector<TopoDS_Edge> splitEdges;
        splitEdges.reserve(uniqueEdgeMap.Extent());
        for (Standard_Integer i = 1; i <= uniqueEdgeMap.Extent(); ++i)
            splitEdges.push_back(TopoDS::Edge(uniqueEdgeMap(i)));

        // 第 3 步：建立无向图。图节点是端点，图边是 OCC 分段 Edge。
        std::vector<FaceGraphNode> nodes;
        std::vector<FaceGraphEdgeNodes> edgeNodes(splitEdges.size());

        for (std::size_t edgeIndex = 0; edgeIndex < splitEdges.size(); ++edgeIndex)
        {
            if (!GetFaceGraphEdgeNodes(
                splitEdges[edgeIndex], tolerance, nodes, edgeNodes[edgeIndex]))
                return false;

            // 将当前分段边登记到首、尾节点的邻接表中。
            nodes[edgeNodes[edgeIndex].First].IncidentEdges.push_back(
                static_cast<int>(edgeIndex));
            nodes[edgeNodes[edgeIndex].Last].IncidentEdges.push_back(
                static_cast<int>(edgeIndex));
        }

        // activeEdges 标记边是否保留，degrees 保存每个节点当前连接的边数。
        std::vector<Standard_Boolean> activeEdges(
            splitEdges.size(), Standard_True);
        std::vector<int> degrees(nodes.size(), 0);
        // 度小于 2 的节点不可能位于完整闭环中，先放入待裁剪队列。
        std::queue<int> pendingNodes;

        for (std::size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
        {
            degrees[nodeIndex] =
                static_cast<int>(nodes[nodeIndex].IncidentEdges.size());
            if (degrees[nodeIndex] < 2)
                pendingNodes.push(static_cast<int>(nodeIndex));
        }

        // 第 4 步：递归剥离悬空枝条。删除一条边后，相邻节点可能成为新的叶子节点，
        // 所以持续处理，直到所有不属于闭环的多余边都被裁掉。
        while (!pendingNodes.empty())
        {
            const int nodeIndex = pendingNodes.front();
            pendingNodes.pop();

            if (degrees[nodeIndex] >= 2)
                continue;

            const std::vector<int>& incidentEdges =
                nodes[nodeIndex].IncidentEdges;

            for (std::size_t i = 0; i < incidentEdges.size(); ++i)
            {
                const int edgeIndex = incidentEdges[i];
                if (!activeEdges[edgeIndex])
                    continue;

                // 仅把临时分段标记为删除，不会删除用户原始线条。
                activeEdges[edgeIndex] = Standard_False;
                const int connectedNodes[2] = {
                    edgeNodes[edgeIndex].First,
                    edgeNodes[edgeIndex].Last
                };

                for (int side = 0; side < 2; ++side)
                {
                    const int connectedNode = connectedNodes[side];
                    if (degrees[connectedNode] > 0)
                        --degrees[connectedNode];
                    if (degrees[connectedNode] < 2)
                        pendingNodes.push(connectedNode);
                }
            }
        }

        // 第 5 步：重新统计裁剪后线网中每个节点的度。
        std::vector<int> finalDegrees(nodes.size(), 0);
        Standard_Integer remainingEdgeCount = 0;
        for (std::size_t edgeIndex = 0; edgeIndex < splitEdges.size(); ++edgeIndex)
        {
            if (!activeEdges[edgeIndex])
                continue;

            ++remainingEdgeCount;
            ++finalDegrees[edgeNodes[edgeIndex].First];
            ++finalDegrees[edgeNodes[edgeIndex].Last];
        }

        if (remainingEdgeCount == 0)
            return false;

        // 闭合轮廓中的有效节点必须恰好连接两条边，即节点度等于 2。
        // 度大于 2 表示存在三岔、四岔或多个候选区域，此时拒绝自动选面以免生成错误。
        for (std::size_t nodeIndex = 0; nodeIndex < finalDegrees.size(); ++nodeIndex)
        {
            if (finalDegrees[nodeIndex] != 0 && finalDegrees[nodeIndex] != 2)
                return false;
        }

        // 第 6 步：收集确认属于闭合区域的分段边。
        Handle(TopTools_HSequenceOfShape) boundaryEdges =
            new TopTools_HSequenceOfShape();
        for (std::size_t edgeIndex = 0; edgeIndex < splitEdges.size(); ++edgeIndex)
        {
            if (activeEdges[edgeIndex])
                boundaryEdges->Append(splitEdges[edgeIndex]);
        }

        // 按统一容差把保留下来的分段边首尾连接成 Wire。
        wires = new TopTools_HSequenceOfShape();
        ShapeAnalysis_FreeBounds::ConnectEdgesToWires(
            boundaryEdges, tolerance, Standard_False, wires);

        if (wires.IsNull() || wires->Length() == 0)
            return false;

        // 最后确认每个 Wire 都真正闭合，否则不能用于制面。
        for (Standard_Integer i = 1; i <= wires->Length(); ++i)
        {
            const TopoDS_Wire wire = TopoDS::Wire(wires->Value(i));
            if (!wire.Closed())
                return false;
        }

        return true;
    }
    // 捕获 OCC 几何异常，避免异常线条导致界面程序崩溃。
    catch (const Standard_Failure&)
    {
        return false;
    }
}