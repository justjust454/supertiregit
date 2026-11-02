// 构造直线绘制接口返回给 C# 层的结果。
// result[0] 保存操作状态，result[1] 保存新创建直线的 Shape ID；
// 尚未生成永久直线时，shapeId 通常为 -1。
static array<int>^ MakeLineCreateResult(
    const int status,
    const int shapeId)
{
    // 使用托管数组，以便 C# 可以直接按 int[] 接收结果。
    array<int>^ result = gcnew array<int>(2);
    result[0] = status;
    result[1] = shapeId;
    return result;
}

// 清理直线绘制过程中使用的临时显示对象。
// removeFirstMarker 为 Standard_False 时，只删除鼠标点标记和动态预览线，
// 保留已经确定的第一点；为 Standard_True 时，第一点标记也一并删除。
static void RemoveLineCreatePreview(
    const Handle(AIS_InteractiveContext)& context,
    const Standard_Boolean removeFirstMarker)
{
    // AIS 对象必须先从交互上下文中移除，否则它们仍会显示在视口中。
    if (!context.IsNull())
    {
        // 删除从第一点连接到当前鼠标位置的橙色预览线。
        if (!GlobalLineCreatePreviewLine.IsNull())
            context->Remove(GlobalLineCreatePreviewLine, Standard_False);

        // 删除随鼠标移动的当前落点标记。
        if (!GlobalLineCreateCursorMarker.IsNull())
            context->Remove(GlobalLineCreateCursorMarker, Standard_False);

        // 取消当前直线或结束绘制模式时，还需要删除第一点标记。
        if (removeFirstMarker && !GlobalLineCreateFirstMarker.IsNull())
            context->Remove(GlobalLineCreateFirstMarker, Standard_False);
    }

    // Remove() 只负责视口中的对象，这里还要释放对应的 Handle，
    // 避免后续代码误以为旧的预览对象仍然有效。
    GlobalLineCreatePreviewLine.Nullify();
    GlobalLineCreateCursorMarker.Nullify();

    if (removeFirstMarker)
        GlobalLineCreateFirstMarker.Nullify();
}

// 在指定三维坐标处创建一个 AIS 点标记，用于显示：
// 1. 当前鼠标投影点；2. 吸附点；3. 已确定的直线第一点。
static Handle(AIS_Point) DisplayLineCreatePoint(
    const Handle(AIS_InteractiveContext)& context,
    const gp_Pnt& point,
    const Quantity_Color& color,
    const Aspect_TypeOfMarker markerType)
{
    // AIS_Point 需要以 Geom_CartesianPoint 作为几何数据。
    Handle(AIS_Point) marker =
        new AIS_Point(new Geom_CartesianPoint(point));

    marker->SetColor(color);
    marker->SetMarker(markerType);

    // 点标记只负责提示，不应参与正常的鼠标选择，因此显示后立即停用选择。
    context->Display(marker, Standard_False);
    context->Deactivate(marker);
    return marker;
}

// 在当前所有可见几何对象中，为鼠标投影点寻找吸附位置。
// 吸附规则为：端点优先，其次是线段上的最近点。
// 找到吸附点时将坐标写入 snappedPoint，并返回 Standard_True；
// snapDistance 范围内没有候选位置时返回 Standard_False。
static Standard_Boolean FindLineCreateSnapPoint(
    const Handle(AIS_InteractiveContext)& context,
    const gp_Pnt& cursorPoint,
    const Standard_Real snapDistance,
    gp_Pnt& snappedPoint)
{
    Standard_Real bestVertexDistance = snapDistance;
    Standard_Boolean foundVertex = Standard_False;

    // 第一遍只检查顶点，使已有直线端点拥有最高吸附优先级。
    for (auto iterator = GlobalShapeMap.begin();
        iterator != GlobalShapeMap.end();
        ++iterator)
    {
        // 隐藏对象不应影响用户当前看到的图纸，因此不参与吸附。
        if (iterator->second.IsNull() ||
            !context->IsDisplayed(iterator->second))
        {
            continue;
        }

        // GlobalShapeMap 中还可能保存其他 AIS 对象；
        // 只有 AIS_Shape 才能继续提取 TopoDS 顶点和边。
        Handle(AIS_Shape) aisShape =
            Handle(AIS_Shape)::DownCast(iterator->second);

        if (aisShape.IsNull() || aisShape->Shape().IsNull())
            continue;

        // 收集当前 Shape 中的全部拓扑顶点，并自动去除重复顶点。
        TopTools_IndexedMapOfShape vertices;
        TopExp::MapShapes(aisShape->Shape(), TopAbs_VERTEX, vertices);

        for (Standard_Integer index = 1;
            index <= vertices.Extent();
            ++index)
        {
            const gp_Pnt point =
                BRep_Tool::Pnt(TopoDS::Vertex(vertices(index)));
            const Standard_Real distance = cursorPoint.Distance(point);

            // 始终保留吸附范围内距离鼠标最近的端点。
            if (distance <= bestVertexDistance)
            {
                bestVertexDistance = distance;
                snappedPoint = point;
                foundVertex = Standard_True;
            }
        }
    }

    // 只要找到端点就立即返回，不再允许普通边吸附覆盖端点结果。
    if (foundVertex)
        return Standard_True;

    // 没有找到端点时，将鼠标投影点转换为拓扑顶点，
    // 供 BRepExtrema_DistShapeShape 计算它到每条边的最短距离。
    const TopoDS_Vertex cursorVertex =
        BRepBuilderAPI_MakeVertex(cursorPoint).Vertex();
    Standard_Real bestEdgeDistance = snapDistance;
    Standard_Boolean foundEdge = Standard_False;

    // 第二遍检查所有可见 Shape 中的边，寻找线段上的最近投影点。
    for (auto iterator = GlobalShapeMap.begin();
        iterator != GlobalShapeMap.end();
        ++iterator)
    {
        if (iterator->second.IsNull() ||
            !context->IsDisplayed(iterator->second))
        {
            continue;
        }

        Handle(AIS_Shape) aisShape =
            Handle(AIS_Shape)::DownCast(iterator->second);

        if (aisShape.IsNull() || aisShape->Shape().IsNull())
            continue;

        // 一个 Shape 可能是 Edge、Wire、Face 或 Compound，
        // MapShapes 会统一提取其中包含的全部 TopoDS_Edge。
        TopTools_IndexedMapOfShape edges;
        TopExp::MapShapes(aisShape->Shape(), TopAbs_EDGE, edges);

        for (Standard_Integer index = 1;
            index <= edges.Extent();
            ++index)
        {
            // 计算鼠标点到当前有限边的真实最短距离及边上的对应点。
            BRepExtrema_DistShapeShape distance(cursorVertex, edges(index));
            distance.Perform();

            // 计算失败、没有解或者距离超过当前最佳值时忽略该边。
            if (!distance.IsDone() ||
                distance.NbSolution() == 0 ||
                distance.Value() > bestEdgeDistance)
            {
                continue;
            }

            // 保存当前更近的边以及该边上的最近点。
            bestEdgeDistance = distance.Value();
            snappedPoint = distance.PointOnShape2(1);
            foundEdge = Standard_True;
        }
    }

    // foundEdge 同时表示 snappedPoint 中是否已经写入了有效吸附坐标。
    return foundEdge;
}
