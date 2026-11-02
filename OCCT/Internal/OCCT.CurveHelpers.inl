//辅助函数：判断两根线是否在 0.01mm 容差内首尾相接 ---国延林 2026.6
static bool AreEdgesTouching(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2, double tol)
{
    for (TopExp_Explorer ex1(shape1, TopAbs_VERTEX); ex1.More(); ex1.Next()) {
        gp_Pnt p1 = BRep_Tool::Pnt(TopoDS::Vertex(ex1.Current()));
        for (TopExp_Explorer ex2(shape2, TopAbs_VERTEX); ex2.More(); ex2.Next()) {
            gp_Pnt p2 = BRep_Tool::Pnt(TopoDS::Vertex(ex2.Current()));
            if (p1.Distance(p2) <= tol) return true;
        }
    }
    return false;
}

struct SnapSplitPlan
{
    std::vector<TopoDS_Edge> TargetEdges;
    TopoDS_Edge SnappedToolEdge;
    Standard_Real Gap;
};

static bool GetOnlyEdge(const TopoDS_Shape& shape, TopoDS_Edge& edge)
{
    TopExp_Explorer explorer(shape, TopAbs_EDGE);
    if (!explorer.More())
        return false;

    edge = TopoDS::Edge(explorer.Current());
    explorer.Next();
    return !explorer.More() && !edge.IsNull();
}
static bool BuildSnapSplitPlan(
    const TopoDS_Shape& targetShape,
    const TopoDS_Shape& toolShape,
    const Standard_Real tolerance,
    SnapSplitPlan& plan)
{
    if (tolerance <= Precision::Confusion())
        return false;

    TopoDS_Edge toolEdge;
    if (!GetOnlyEdge(toolShape, toolEdge))
        return false;

    BRepAdaptor_Curve toolAdaptor(toolEdge);
    if (toolAdaptor.GetType() != GeomAbs_Line)
        return false;

    Standard_Real toolFirst = 0.0;
    Standard_Real toolLast = 0.0;
    Handle(Geom_Curve) toolCurve = BRep_Tool::Curve(toolEdge, toolFirst, toolLast);
    if (toolCurve.IsNull())
        return false;

    const gp_Pnt toolStart = toolCurve->Value(toolFirst);
    const gp_Pnt toolEnd = toolCurve->Value(toolLast);
    const Standard_Real minPieceLength =
        std::max(tolerance * 0.1, Precision::Confusion() * 100.0);
    const Standard_Real supportDistanceTolerance =
        std::max(tolerance * 0.01, Precision::Confusion() * 100.0);
    const Standard_Real minAngle = 3.0 * 3.14159265358979323846 / 180.0;

    Standard_Real bestGap = std::numeric_limits<Standard_Real>::max();
    TopoDS_Edge bestTargetEdge;
    gp_Pnt bestSnapPoint;
    bool snapToolStart = false;

    try
    {
        for (TopExp_Explorer explorer(targetShape, TopAbs_EDGE);
            explorer.More(); explorer.Next())
        {
            const TopoDS_Edge targetEdge = TopoDS::Edge(explorer.Current());
            BRepAdaptor_Curve targetAdaptor(targetEdge);
            if (targetAdaptor.GetType() != GeomAbs_Line)
                continue;

            const Standard_Real angle =
                targetAdaptor.Line().Direction().Angle(toolAdaptor.Line().Direction());
            const Standard_Real acuteAngle = std::min(angle, 3.14159265358979323846 - angle);
            if (acuteAngle < minAngle)
                continue;

            Extrema_ExtElC extrema(
                targetAdaptor.Line(), toolAdaptor.Line(), Precision::Angular());
            if (!extrema.IsDone() || extrema.IsParallel() || extrema.NbExt() < 1)
                continue;

            for (Standard_Integer i = 1; i <= extrema.NbExt(); ++i)
            {
                if (extrema.SquareDistance(i) >
                    supportDistanceTolerance * supportDistanceTolerance)
                    continue;

                Extrema_POnCurv targetPointOnCurve;
                Extrema_POnCurv toolPointOnCurve;
                extrema.Points(i, targetPointOnCurve, toolPointOnCurve);

                Standard_Real targetFirst = 0.0;
                Standard_Real targetLast = 0.0;
                Handle(Geom_Curve) targetCurve =
                    BRep_Tool::Curve(targetEdge, targetFirst, targetLast);
                if (targetCurve.IsNull())
                    continue;

                GeomAPI_ProjectPointOnCurve projector(
                    targetPointOnCurve.Value(), targetCurve, targetFirst, targetLast);
                if (projector.NbPoints() < 1)
                    continue;

                const gp_Pnt snapPoint = projector.NearestPoint();
                if (snapPoint.Distance(targetPointOnCurve.Value()) > supportDistanceTolerance)
                    continue;

                const gp_Pnt targetStart = targetCurve->Value(targetFirst);
                const gp_Pnt targetEnd = targetCurve->Value(targetLast);
                if (targetStart.Distance(snapPoint) <= minPieceLength ||
                    targetEnd.Distance(snapPoint) <= minPieceLength)
                    continue;

                const Standard_Real startGap = toolStart.Distance(snapPoint);
                const Standard_Real endGap = toolEnd.Distance(snapPoint);
                const bool useStart = startGap <= endGap;
                const Standard_Real gap = useStart ? startGap : endGap;

                if (gap > tolerance || gap >= bestGap)
                    continue;

                const gp_Pnt fixedToolEnd = useStart ? toolEnd : toolStart;
                if (fixedToolEnd.Distance(snapPoint) <= minPieceLength)
                    continue;

                bestGap = gap;
                bestTargetEdge = targetEdge;
                bestSnapPoint = snapPoint;
                snapToolStart = useStart;
            }
        }

        if (bestTargetEdge.IsNull())
            return false;

        plan.TargetEdges.clear();
        for (TopExp_Explorer explorer(targetShape, TopAbs_EDGE);
            explorer.More(); explorer.Next())
        {
            const TopoDS_Edge targetEdge = TopoDS::Edge(explorer.Current());
            if (!targetEdge.IsSame(bestTargetEdge))
            {
                plan.TargetEdges.push_back(targetEdge);
                continue;
            }

            Standard_Real targetFirst = 0.0;
            Standard_Real targetLast = 0.0;
            Handle(Geom_Curve) targetCurve =
                BRep_Tool::Curve(targetEdge, targetFirst, targetLast);
            if (targetCurve.IsNull())
                return false;

            BRepBuilderAPI_MakeEdge firstPart(targetCurve->Value(targetFirst), bestSnapPoint);
            BRepBuilderAPI_MakeEdge secondPart(bestSnapPoint, targetCurve->Value(targetLast));
            if (!firstPart.IsDone() || !secondPart.IsDone())
                return false;

            plan.TargetEdges.push_back(firstPart.Edge());
            plan.TargetEdges.push_back(secondPart.Edge());
        }

        BRepBuilderAPI_MakeEdge snappedToolMaker(
            snapToolStart ? bestSnapPoint : toolStart,
            snapToolStart ? toolEnd : bestSnapPoint);
        if (!snappedToolMaker.IsDone())
            return false;

        plan.SnappedToolEdge = snappedToolMaker.Edge();
        plan.Gap = bestGap;
        return plan.TargetEdges.size() >= 2 && !plan.SnappedToolEdge.IsNull();
    }
    catch (const Standard_Failure&)
    {
        return false;
    }
}