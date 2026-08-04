// ============================================================================
// Active headers, grouped by responsibility.
// Legacy headers are preserved in the disabled block below for easy recovery.
// ============================================================================

#pragma region CppCli_And_Standard_Library
#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>
#include <map>
#include <vector>
#include <vcclr.h>
#include <NCollection_Haft.h>
#pragma endregion

#pragma region OCCT_Core_And_Geometry
#include <Standard.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Failure.hxx>
#include <Standard_Handle.hxx>
#include <Standard_Integer.hxx>
#include <Standard_Version.hxx>
#include <Precision.hxx>

#include <gp_Ax2.hxx>
#include <gp_Circ.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>

#include <Geom_Curve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomAbs_CurveType.hxx>
#include <GeomConvert_CompCurveToBSplineCurve.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <GCPnts_UniformAbscissa.hxx>
#include <TColgp_HArray1OfPnt.hxx>

#include <BRepAdaptor_Curve.hxx>
#include <Extrema_ExtElC.hxx>
#include <Extrema_POnCurv.hxx>
#pragma endregion

#pragma region OCCT_Topology_And_Modeling
#include <TopAbs_ShapeEnum.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopLoc_Location.hxx>

#include <BRep_Tool.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <ShapeAnalysis_FreeBounds.hxx>

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Splitter.hxx>
#include <BOPAlgo_Builder.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <BRepGProp.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <GProp_GProps.hxx>

#include <TopTools_HSequenceOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#pragma endregion

#pragma region OCCT_Viewer
#include <Aspect_DisplayConnection.hxx>
#include <WNT_Window.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

#include <AIS_InteractiveContext.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <AIS_ListOfInteractive.hxx>
#include <AIS_Shape.hxx>
#include <AIS_ViewCube.hxx>

#include <Graphic3d_RenderingParams.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_LineAspect.hxx>
#include <Quantity_Color.hxx>
#pragma endregion

#pragma region STEP_XCAF_Export
#include <IFSelect_ReturnStatus.hxx>
#include <Interface_Static.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPCAFControl_Writer.hxx>

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TDataStd_Name.hxx>
#include <TDF_Label.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#pragma endregion

#if 0 // UNUSED_LEGACY_HEADERS
// Generic and duplicate legacy headers
#include <iostream>
#include <iomanip>
#include <set>
#include <Standard_DefineAlloc.hxx>
#include <Standard_OStream.hxx>
#include <Standard_Type.hxx>
#include <Standard_ConstructionError.hxx>
#include <Standard_DomainError.hxx>
#include <Standard_OutOfRange.hxx>
#include <gp_VectorWithNullMagnitude.hxx>
#include <gp_XYZ.hxx>
#include <Geom_Line.hxx>
#include <Geom_Circle.hxx>
#include <TColgp_HSequenceOfPnt.hxx>
#include <TopTools_MapOfShape.hxx>
#include <TopTools_SequenceOfShape.hxx>

// Legacy import/export headers
#include <BRepTools.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <XCAFPrs_AISObject.hxx>
#include <Resource_FormatType.hxx>
#include <StlAPI_Writer.hxx>
#include <VrmlAPI_Writer.hxx>

// Legacy primitive and interactive modeling headers
#include <BRepBuilderAPI_MakeShape.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeWedge.hxx>
#include <BRepPrimAPI_MakeOneAxis.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <ShapeFix_Shape.hxx>
#include <gce_MakeLin.hxx>
#include <gce_MakePln.hxx>
#include <ProjLib.hxx>
#include <ElSLib.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Fuse.hxx>

// Legacy progress and XCAF reader internals
#include <Message_ProgressIndicator.hxx>
#include <Message_ProgressScope.hxx>
#include <Graphic3d_NameOfMaterial.hxx>
#include <TDF_AttributeIndexedMap.hxx>
#include <TDF_Transaction.hxx>
#include <TDF_DeltaList.hxx>
#include <CDM_Document.hxx>
#include <TDF_LabelMap.hxx>
#include <TDocStd_FormatVersion.hxx>
#include <XCAFDoc_DataMapOfShapeLabel.hxx>
#include <TDataStd_NamedData.hxx>
#include <TDataStd_GenericEmpty.hxx>
#include <TDF_LabelSequence.hxx>
#include <TColStd_SequenceOfHAsciiString.hxx>
#include <TDF_AttributeSequence.hxx>
#include <STEPCAFControl_DataMapOfShapePD.hxx>
#include <STEPCAFControl_DataMapOfPDExternFile.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include <XCAFDimTolObjects_DatumModifiersSequence.hxx>
#include <XCAFDimTolObjects_DatumModifWithValue.hxx>
#include <TDF_LabelNodePtr.hxx>
#endif // UNUSED_LEGACY_HEADERS
static std::map<int, Handle(AIS_InteractiveObject)> GlobalShapeMap;

// list of required OCCT libraries
#pragma comment(lib, "TKernel.lib")
#pragma comment(lib, "TKMath.lib")
#pragma comment(lib, "TKBRep.lib")
#pragma comment(lib, "TKXSBase.lib")
#pragma comment(lib, "TKService.lib")
#pragma comment(lib, "TKV3d.lib")
#pragma comment(lib, "TKOpenGl.lib")
#pragma comment(lib, "TKIGES.lib")
#pragma comment(lib, "TKSTEP.lib")
#pragma comment(lib, "TKStl.lib")
#pragma comment(lib, "TKVrml.lib")
#pragma comment(lib, "TKLCAF.lib")
#pragma comment(lib, "TKTopAlgo.lib")//BOX所需Lib
#pragma comment(lib, "TKPrim.lib")
#pragma comment(lib, "TKGeomBase.lib")
#pragma comment(lib, "TKBin.lib")
#pragma comment(lib, "TKBinL.lib")
#pragma comment(lib, "TKBinTObj.lib")
#pragma comment(lib, "TKBinXCAF.lib")
#pragma comment(lib, "TKBO.lib")
#pragma comment(lib, "TKBool.lib")
#pragma comment(lib, "TKCAF.lib")
#pragma comment(lib, "TKCDF.lib")
#pragma comment(lib, "TKDCAF.lib")
#pragma comment(lib, "TKDFBrowser.lib")
#pragma comment(lib, "TKDraw.lib")
#pragma comment(lib, "TKFeat.lib")
#pragma comment(lib, "TKFillet.lib")
#pragma comment(lib, "TKG2d.lib")//检索边所需要的静态链接库
#pragma comment(lib, "TKG3d.lib")//检索边所需要的静态链接库
#pragma comment(lib, "TKMesh.lib")
#pragma comment(lib, "TKXCAF.lib")
#pragma comment(lib, "TKXDESTEP.lib")

#pragma comment(lib, "TKSTEP.lib")
#pragma comment(lib, "TKSTEPBase.lib")
#pragma comment(lib, "TKSTEPAttr.lib")
#pragma comment(lib, "TKSTEP209.lib")


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

//! Auxiliary tool for converting C# string into UTF-8 string.
static TCollection_AsciiString toAsciiString(String^ theString)
{
    if (theString == nullptr)
    {
        return TCollection_AsciiString();
    }

    pin_ptr<const wchar_t> aPinChars = PtrToStringChars(theString);
    const wchar_t* aWCharPtr = aPinChars;
    if (aWCharPtr == NULL
        || *aWCharPtr == L'\0')
    {
        return TCollection_AsciiString();
    }
    return TCollection_AsciiString(aWCharPtr);
}

/*struct BoundaryTraceItem
{
    int id;
    TopoDS_Edge edge;
    gp_Pnt fromP;
    gp_Pnt toP;
};*/

/*static bool GetFirstEdgeFromAISObject(
    const Handle(AIS_InteractiveObject)& obj,
    TopoDS_Edge& outEdge)
{
    if (obj.IsNull())
        return false;

    Handle(AIS_Shape) aisShape = Handle(AIS_Shape)::DownCast(obj);

    if (aisShape.IsNull())
        return false;

    TopoDS_Shape shape = aisShape->Shape();

    if (shape.IsNull())
        return false;

    TopExp_Explorer exp(shape, TopAbs_EDGE);

    if (!exp.More())
        return false;

    outEdge = TopoDS::Edge(exp.Current());

    if (outEdge.IsNull())
        return false;

    return true;
}*/

/*static bool GetEdgeEndPoints(
    const TopoDS_Edge& edge,
    gp_Pnt& p0,
    gp_Pnt& p1)
{
    if (edge.IsNull())
        return false;

    TopoDS_Vertex v0 = TopExp::FirstVertex(edge);
    TopoDS_Vertex v1 = TopExp::LastVertex(edge);

    if (v0.IsNull() || v1.IsNull())
        return false;

    p0 = BRep_Tool::Pnt(v0);
    p1 = BRep_Tool::Pnt(v1);

    if (p0.Distance(p1) <= Precision::Confusion())
        return false;

    return true;
}

static bool GetCandidateLeavingDirection(
    const TopoDS_Edge& edge,
    const gp_Pnt& currentPoint,
    const Standard_Real snapTol,
    gp_Pnt& otherPoint,
    gp_Vec& leavingDir,
    gp_Pnt& fromPoint,
    gp_Pnt& toPoint)
{
    gp_Pnt p0;
    gp_Pnt p1;

    if (!GetEdgeEndPoints(edge, p0, p1))
        return false;

    if (currentPoint.Distance(p0) <= snapTol)
    {
        gp_Vec dir(p0, p1);

        if (dir.SquareMagnitude() < 1.0e-18)
            return false;

        dir.Normalize();

        otherPoint = p1;
        leavingDir = dir;
        fromPoint = p0;
        toPoint = p1;

        return true;
    }

    if (currentPoint.Distance(p1) <= snapTol)
    {
        gp_Vec dir(p1, p0);

        if (dir.SquareMagnitude() < 1.0e-18)
            return false;

        dir.Normalize();

        otherPoint = p0;
        leavingDir = dir;
        fromPoint = p1;
        toPoint = p0;

        return true;
    }

    return false;
}

static void ReverseTraceItem(
    const BoundaryTraceItem& src,
    BoundaryTraceItem& dst)
{
    dst.id = src.id;
    dst.edge = src.edge;
    dst.fromP = src.toP;
    dst.toP = src.fromP;
}

static Standard_Boolean TraceOneSideByDirection(
    const Handle(AIS_InteractiveContext)& ctx,
    const gp_Pnt& traceStartPoint,
    const gp_Vec& traceStartDir,
    const Standard_Real initialStepLen,
    const Standard_Real snapTol,
    const Standard_Real maxTurnAngleDeg,
    std::set<int>& usedIds,
    std::vector<BoundaryTraceItem>& outItems)
{
    if (ctx.IsNull())
        return Standard_False;

    gp_Pnt currentPoint = traceStartPoint;
    gp_Vec currentDir = traceStartDir;

    if (currentDir.SquareMagnitude() < 1.0e-18)
        return Standard_False;

    currentDir.Normalize();

    Standard_Real prevStepLen = initialStepLen;
    if (prevStepLen < snapTol * 5.0)
        prevStepLen = snapTol * 20.0;

    const Standard_Real pi = 3.14159265358979323846;
    const Standard_Real minContinueDot =
        std::cos(maxTurnAngleDeg * pi / 180.0);

    const Standard_Integer maxStepCount = 10000;

    for (Standard_Integer step = 0; step < maxStepCount; ++step)
    {
        Standard_Real bestScore = 1.0e100;
        Standard_Real secondBestScore = 1.0e100;

        int bestId = -1;
        TopoDS_Edge bestEdge;
        gp_Pnt bestOtherPoint;
        gp_Pnt bestFromPoint;
        gp_Pnt bestToPoint;
        gp_Vec bestLeavingDir;
        Standard_Real bestLen = 0.0;

        for (auto const& pair : GlobalShapeMap)
        {
            int candId = pair.first;

            if (usedIds.find(candId) != usedIds.end())
                continue;

            Handle(AIS_InteractiveObject) candObj = pair.second;

            if (candObj.IsNull())
                continue;

            if (!ctx->IsDisplayed(candObj))
                continue;

            TopoDS_Edge candEdge;
            if (!GetFirstEdgeFromAISObject(candObj, candEdge))
                continue;

            gp_Pnt otherPoint;
            gp_Pnt fromPoint;
            gp_Pnt toPoint;
            gp_Vec leavingDir;

            if (!GetCandidateLeavingDirection(
                candEdge,
                currentPoint,
                snapTol,
                otherPoint,
                leavingDir,
                fromPoint,
                toPoint))
            {
                continue;
            }

            Standard_Real dot = currentDir.Dot(leavingDir);

            // 第一关：方向至少要基本顺
            if (dot < minContinueDot)
                continue;

            Standard_Real candLen = fromPoint.Distance(toPoint);
            if (candLen <= snapTol)
                continue;

            // 向前预测点：真正该走的边，通常应该更接近“沿当前方向继续前进”
            gp_Pnt predictedPoint = currentPoint.Translated(currentDir.Multiplied(prevStepLen));
            Standard_Real predErr = otherPoint.Distance(predictedPoint);

            // 方向惩罚：越不顺越差
            Standard_Real dirPenalty = 1.0 - dot;

            // 预测误差惩罚：越偏离“向前走”越差
            Standard_Real predPenalty = predErr / std::max(prevStepLen, snapTol);

            // 长度惩罚：防止通过很短的小连接线跳到邻近轮廓
            Standard_Real lenPenalty = 0.0;
            if (candLen < prevStepLen * 0.35)
            {
                lenPenalty =
                    (prevStepLen * 0.35 - candLen) /
                    std::max(prevStepLen, snapTol);
            }

            // 太长也稍微惩罚，避免跨接
            Standard_Real longPenalty = 0.0;
            if (candLen > prevStepLen * 2.5)
            {
                longPenalty =
                    (candLen - prevStepLen * 2.5) /
                    std::max(prevStepLen, snapTol);
            }

            // 综合评分：越小越好
            Standard_Real score =
                8.0 * dirPenalty * dirPenalty +
                6.0 * predPenalty * predPenalty +
                4.0 * lenPenalty * lenPenalty +
                2.0 * longPenalty * longPenalty;

            if (score < bestScore)
            {
                secondBestScore = bestScore;

                bestScore = score;
                bestId = candId;
                bestEdge = candEdge;
                bestOtherPoint = otherPoint;
                bestLeavingDir = leavingDir;
                bestFromPoint = fromPoint;
                bestToPoint = toPoint;
                bestLen = candLen;
            }
            else if (score < secondBestScore)
            {
                secondBestScore = score;
            }
        }

        if (bestId < 0)
            break;

        // 分叉保护：
        // 如果第一名和第二名太接近，说明这里很可能是岔路口。
        // 宁可停，也不要跳到旁边那条线。
        if (secondBestScore < 1.0e99)
        {
            if (std::fabs(bestScore - secondBestScore) < 0.08)
            {
                break;
            }
        }

        BoundaryTraceItem item;
        item.id = bestId;
        item.edge = bestEdge;
        item.fromP = bestFromPoint;
        item.toP = bestToPoint;

        outItems.push_back(item);
        usedIds.insert(bestId);

        currentPoint = bestOtherPoint;
        currentDir = bestLeavingDir;
        currentDir.Normalize();

        // 平滑更新“典型步长”
        prevStepLen = 0.65 * prevStepLen + 0.35 * bestLen;
    }

    return outItems.empty() ? Standard_False : Standard_True;
}



static Handle(Geom_TrimmedCurve) MakeTrimmedCurveForTraceItem(
    const BoundaryTraceItem& item,
    const Standard_Real reverseCheckTol)
{
    Standard_Real f = 0.0;
    Standard_Real l = 0.0;

    TopLoc_Location loc;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(item.edge, loc, f, l);

    if (curve.IsNull())
        return NULL;

    if (!loc.IsIdentity())
    {
        Handle(Geom_Geometry) g = curve->Transformed(loc.Transformation());
        curve = Handle(Geom_Curve)::DownCast(g);

        if (curve.IsNull())
            return NULL;
    }

    if (l <= f)
        return NULL;

    Handle(Geom_TrimmedCurve) trimmed =
        new Geom_TrimmedCurve(curve, f, l);

    gp_Pnt cStart = trimmed->StartPoint();
    gp_Pnt cEnd = trimmed->EndPoint();

    // 让曲线方向与追踪方向 item.fromP -> item.toP 一致。
    // 不依赖 Edge 的 Orientation，而是用实际几何端点判断，最稳。
    Standard_Real startDist = cStart.Distance(item.fromP);
    Standard_Real endDist = cEnd.Distance(item.fromP);

    if (endDist + reverseCheckTol < startDist)
    {
        trimmed->Reverse();
    }

    return trimmed;
}


static Handle(Geom_BSplineCurve) BuildSingleBSplineFromOrderedTraceItems(
    const std::vector<BoundaryTraceItem>& orderedItems,
    const Standard_Real concatTol)
{
    if (orderedItems.empty())
        return NULL;

    Handle(Geom_TrimmedCurve) firstCurve =
        MakeTrimmedCurveForTraceItem(orderedItems[0], concatTol);

    if (firstCurve.IsNull())
        return NULL;

    GeomConvert_CompCurveToBSplineCurve concatenator(firstCurve);

    for (std::size_t i = 1; i < orderedItems.size(); ++i)
    {
        Handle(Geom_TrimmedCurve) nextCurve =
            MakeTrimmedCurveForTraceItem(orderedItems[i], concatTol);

        if (nextCurve.IsNull())
            return NULL;

        Standard_Boolean okAdd =
            concatenator.Add(nextCurve, concatTol, Standard_True);

        if (!okAdd)
            return NULL;
    }

    return concatenator.BSplineCurve();
}*/










/// <summary>
/// Proxy class encapsulating calls to OCCT C++ classes within 
/// C++/CLI class visible from .Net (CSharp)
/// </summary>
public ref class OCCTProxy
{
public:
    // ============================================
    // Viewer functionality
    // ============================================

    /// <summary>
    ///Initialize a viewer
    /// </summary>
    /// <param name="theWnd">System.IntPtr that contains the window handle (HWND) of the control</param>

// OLD_REGION: #pragma region OCC初始化函数，初始化视图、颜色、鼠标键盘响应函数，杨宇昂添加，2024.10
#pragma region 01_Viewer_Initialization_Interaction_And_DisplayQuality
    bool InitViewer(System::IntPtr theWnd)
    {
        try
        {
            Handle(Aspect_DisplayConnection) aDisplayConnection;
            myGraphicDriver() = new OpenGl_GraphicDriver(aDisplayConnection);
        }
        catch (Standard_Failure)
        {
            return false;
        }

        myViewer() = new V3d_Viewer(myGraphicDriver());
        myViewer()->SetDefaultLights();
        myViewer()->SetLightOn();
        myView() = myViewer()->CreateView();
        Handle(WNT_Window) aWNTWindow = new WNT_Window(reinterpret_cast<HWND> (theWnd.ToPointer()));
        myView()->SetWindow(aWNTWindow);
        if (!aWNTWindow->IsMapped())
        {
            aWNTWindow->Map();
        }
        myAISContext() = new AIS_InteractiveContext(myViewer());
        ApplyOCCTHighQualityDisplay();
        myAISContext()->UpdateCurrentViewer();
        myView()->Redraw();
        myView()->MustBeResized();
        //通过提供颜色和填充方法(默认为水平)来定义视图的渐变背景色
        //myView()->SetBgGradientColors(Quantity_NOC_GRAY, Quantity_NOC_WHITE, Aspect_GFM_VER);
        //**
        //视图导航器 
        Handle(AIS_ViewCube) aViewCube = new AIS_ViewCube();
        aViewCube->SetTransformPersistence(new Graphic3d_TransformPers(Graphic3d_TMF_TriedronPers, Aspect_TOTP_RIGHT_UPPER, Graphic3d_Vec2i(100, 100)));
        myAISContext()->Display(aViewCube, Standard_True);
        //**
        return true;
    }
// OLD_REGION: #pragma region 视图一键调整
    /// <summary>
    /// 设置相机的投影方向
    /// </summary>
    /// <param name="dx">X方向向量</param>
    /// <param name="dy">Y方向向量</param>
    /// <param name="dz">Z方向向量</param>
#if 0 // UNUSED_LEGACY: SetViewDirection
    void SetViewDirection(double dx, double dy, double dz)
    {
        if (myView().IsNull()) return;

        // 1. 设置投影方向
        myView()->SetProj(dx, dy, dz);

        // 2. 防颠倒保护
        if (dx == 0 && dy == 0) 
        {
            myView()->SetUp(0, 0, 1);
        }
        else 
        {
            myView()->SetUp(0, 0, 0);
        }

        // 3. 刷新画面与重新计算深度裁剪
        myView()->SetTwist(0.0);
        myView()->FitAll();
        myView()->ZFitAll(); // 防止 2D 图形在 3D 视角下被切掉
        myView()->Redraw();
    }
#endif // UNUSED_LEGACY: SetViewDirection

    /// <summary>
    /// Make dump of current view to file
    /// </summary>
    /// <param name="theFileName">Name of dump file</param>
#if 0 // UNUSED_LEGACY: Dump
    bool Dump(const TCollection_AsciiString& theFileName)
    {
        if (myView().IsNull())
        {
            return false;
        }
        myView()->Redraw();
        return myView()->Dump(theFileName.ToCString()) != Standard_False;
    }
#endif // UNUSED_LEGACY: Dump

    /// <summary>
    ///Redraw view
    /// </summary>
    void RedrawView(void)
    {
        if (!myView().IsNull())
        {
            myView()->Redraw();
        }
    }
    /// <summary>
    ///clear all
    /// </summary>
    //void ClearAll(void)
    //{
    //    if (!myAISContext().IsNull())
    //    {
    //        // EraseAll 参数传 Standard_True 表示立刻更新画面
    //        myAISContext()->EraseAll(Standard_True);
    //    }
    //}


    /// <summary>
    ///Update view
    /// </summary>
    void UpdateView(void)
    {
        if (!myView().IsNull())
        {
            myView()->MustBeResized();
        }
    }

    /// <summary>
    ///Set computed mode in false
    /// </summary>
#if 0 // UNUSED_LEGACY: SetDegenerateModeOn
    void SetDegenerateModeOn(void)
    {
        if (!myView().IsNull())
        {
            myView()->SetComputedMode(Standard_False);
            myView()->Redraw();
        }
    }
#endif // UNUSED_LEGACY: SetDegenerateModeOn

    /// <summary>
    ///Set computed mode in true
    /// </summary>
#if 0 // UNUSED_LEGACY: SetDegenerateModeOff
    void SetDegenerateModeOff(void)
    {
        if (!myView().IsNull())
        {
            myView()->SetComputedMode(Standard_False);
            myView()->Redraw();
        }
    }
#endif // UNUSED_LEGACY: SetDegenerateModeOff

    /// <summary>
    ///Fit all
    /// </summary>
#if 0 // UNUSED_LEGACY: WindowFitAll
    void WindowFitAll(int theXmin, int theYmin, int theXmax, int theYmax)
    {
        if (!myView().IsNull())
        {
            myView()->WindowFitAll(theXmin, theYmin, theXmax, theYmax);
        }
    }
#endif // UNUSED_LEGACY: WindowFitAll

    /// <summary>
    ///Current place of window
    /// </summary>
    /// <param name="theZoomFactor">Current zoom</param>
#if 0 // UNUSED_LEGACY: Place
    void Place(int theX, int theY, float theZoomFactor)
    {
        Standard_Real aZoomFactor = theZoomFactor;
        if (!myView().IsNull())
        {
            myView()->Place(theX, theY, aZoomFactor);
        }
    }
#endif // UNUSED_LEGACY: Place

    /// <summary>
    ///Set Zoom
    /// </summary>
    void Zoom(int theX1, int theY1, int theX2, int theY2)
    {
        if (!myView().IsNull())
        {
            myView()->Zoom(theX1, theY1, theX2, theY2);
        }
    }

    /// <summary>
    ///Set Pan
    /// </summary>
    void Pan(int theX, int theY)
    {
        if (!myView().IsNull())
        {
            myView()->Pan(theX, theY);
        }
    }

    /// <summary>
    ///Rotation
    /// </summary>
    void Rotation(int theX, int theY)
    {
        if (!myView().IsNull())
        {
            myView()->Rotation(theX, theY);
        }
    }

    /// <summary>
    ///Start rotation
    /// </summary>
    void StartRotation(int theX, int theY)
    {
        if (!myView().IsNull())
        {
            myView()->StartRotation(theX, theY);
        }
    }

    /// <summary>
    ///Select by rectangle
    /// </summary>
#if 0 // UNUSED_LEGACY: SelectRectangle
    void Select(int theX1, int theY1, int theX2, int theY2)
    {
        if (!myAISContext().IsNull())
        {
            myAISContext()->SelectRectangle(Graphic3d_Vec2i(theX1, theY1),
                Graphic3d_Vec2i(theX2, theY2),
                myView());
            myAISContext()->UpdateCurrentViewer();
        }
    }
#endif // UNUSED_LEGACY: SelectRectangle

    /// <summary>
    ///Select by click
    /// </summary>
    void Select(void)
    {
        if (!myAISContext().IsNull())
        {
            myAISContext()->SelectDetected();
            myAISContext()->UpdateCurrentViewer();
        }
    }

    /// <summary>
    ///Move view
    /// </summary>
    void MoveTo(int theX, int theY)
    {
        if ((!myAISContext().IsNull()) && (!myView().IsNull()))
        {
            myAISContext()->MoveTo(theX, theY, myView(), Standard_True);
        }
    }

    /// <summary>
    ///Select by rectangle with pressed "Shift" key
    /// </summary>
#if 0 // UNUSED_LEGACY: ShiftSelectRectangle
    void ShiftSelect(int theX1, int theY1, int theX2, int theY2)
    {
        if ((!myAISContext().IsNull()) && (!myView().IsNull()))
        {
            myAISContext()->SelectRectangle(Graphic3d_Vec2i(theX1, theY1),
                Graphic3d_Vec2i(theX2, theY2),
                myView(),
                AIS_SelectionScheme_XOR);
            myAISContext()->UpdateCurrentViewer();
        }
    }
#endif // UNUSED_LEGACY: ShiftSelectRectangle

    /// <summary>
    ///Select by "Shift" key
    /// </summary>
    void ShiftSelect(void)
    {
        if (!myAISContext().IsNull())
        {
            myAISContext()->SelectDetected(AIS_SelectionScheme_XOR);
            myAISContext()->UpdateCurrentViewer();
        }
    }

    /// <summary>
    ///Set background color
    /// </summary>
#if 0 // UNUSED_LEGACY: BackgroundColor
    void BackgroundColor(int& theRed, int& theGreen, int& theBlue)
    {
        Standard_Real R1;
        Standard_Real G1;
        Standard_Real B1;
        if (!myView().IsNull())
        {
            myView()->BackgroundColor(Quantity_TOC_RGB, R1, G1, B1);
        }
        theRed = (int)R1 * 255;
        theGreen = (int)G1 * 255;
        theBlue = (int)B1 * 255;
    }
#endif // UNUSED_LEGACY: BackgroundColor

    /// <summary>
    ///Get background color Red
    /// </summary>
#if 0 // UNUSED_LEGACY: GetBGColR
    int GetBGColR(void)
    {
        int aRed, aGreen, aBlue;
        BackgroundColor(aRed, aGreen, aBlue);
        return aRed;
    }
#endif // UNUSED_LEGACY: GetBGColR

    /// <summary>
    ///Get background color Green
    /// </summary>
#if 0 // UNUSED_LEGACY: GetBGColG
    int GetBGColG(void)
    {
        int aRed, aGreen, aBlue;
        BackgroundColor(aRed, aGreen, aBlue);
        return aGreen;
    }
#endif // UNUSED_LEGACY: GetBGColG

    /// <summary>
    ///Get background color Blue
    /// </summary>
#if 0 // UNUSED_LEGACY: GetBGColB
    int GetBGColB(void)
    {
        int aRed, aGreen, aBlue;
        BackgroundColor(aRed, aGreen, aBlue);
        return aBlue;
    }
#endif // UNUSED_LEGACY: GetBGColB

    /// <summary>
    ///Update current viewer
    /// </summary>
    void UpdateCurrentViewer(void)
    {
        if (!myAISContext().IsNull())
        {
            myAISContext()->UpdateCurrentViewer();
        }
    }

    /// <summary>
    ///Front side
    /// </summary>
#if 0 // UNUSED_LEGACY: FrontView
    void FrontView(void)
    {
        if (!myView().IsNull())
        {
            myView()->SetProj(V3d_Yneg);
        }
    }
#endif // UNUSED_LEGACY: FrontView

    /// <summary>
    ///Top side
    /// </summary>
#if 0 // UNUSED_LEGACY: TopView
    void TopView(void)
    {
        if (!myView().IsNull())
        {
            myView()->SetProj(V3d_Zpos);
        }
    }
#endif // UNUSED_LEGACY: TopView

    /// <summary>
    ///Left side
    /// </summary>
#if 0 // UNUSED_LEGACY: LeftView
    void LeftView(void)
    {
        if (!myView().IsNull())
        {
            myView()->SetProj(V3d_Xneg);
        }
    }
#endif // UNUSED_LEGACY: LeftView

    /// <summary>
    ///Back side
    /// </summary>
#if 0 // UNUSED_LEGACY: BackView
    void BackView(void)
    {
        if (!myView().IsNull())
        {
            myView()->SetProj(V3d_Ypos);
        }
    }
#endif // UNUSED_LEGACY: BackView

    /// <summary>
    ///Right side
    /// </summary>
#if 0 // UNUSED_LEGACY: RightView
    void RightView(void)
    {
        if (!myView().IsNull())
        {
            myView()->SetProj(V3d_Xpos);
        }
    }
#endif // UNUSED_LEGACY: RightView

    /// <summary>
    ///Bottom side
    /// </summary>
#if 0 // UNUSED_LEGACY: BottomView
    void BottomView(void)
    {
        if (!myView().IsNull())
        {
            myView()->SetProj(V3d_Zneg);
        }
    }
#endif // UNUSED_LEGACY: BottomView

    /// <summary>
    ///Axo side
    /// </summary>
#if 0 // UNUSED_LEGACY: AxoView
    void AxoView(void)
    {
        if (!myView().IsNull())
        {
            myView()->SetProj(V3d_XposYnegZpos);
        }
    }
#endif // UNUSED_LEGACY: AxoView

    /// <summary>
    ///Scale
    /// </summary>
#if 0 // UNUSED_LEGACY: Scale
    float Scale(void)
    {
        if (myView().IsNull())
        {
            return -1;
        }
        else
        {
            return (float)myView()->Scale();
        }
    }
#endif // UNUSED_LEGACY: Scale

    /// <summary>
    ///Zoom in all view
    /// </summary>
    void ZoomAllView(void)
    {
        if (!myView().IsNull())
        {
            myView()->FitAll();
            myView()->ZFitAll();
        }
    }

    /// <summary>
    ///Reset view
    /// </summary>
    void Reset(void)
    {
        if (!myView().IsNull())
        {
            myView()->Reset();
        }
    }

    /// <summary>
    ///Set display mode of objects
    /// </summary>
    /// <param name="theMode">Set current mode</param>
#if 0 // UNUSED_LEGACY: SetDisplayMode
    void SetDisplayMode(int theMode)
    {
        if (myAISContext().IsNull())
        {
            return;
        }
        AIS_DisplayMode aCurrentMode;
        if (theMode == 0)
        {
            aCurrentMode = AIS_WireFrame;
        }
        else
        {
            aCurrentMode = AIS_Shaded;
        }

        if (myAISContext()->NbSelected() == 0)
        {
            myAISContext()->SetDisplayMode(aCurrentMode, Standard_False);
        }
        else
        {
            for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
            {
                myAISContext()->SetDisplayMode(myAISContext()->SelectedInteractive(), theMode, Standard_False);
            }
        }
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: SetDisplayMode

    /// <summary>
    ///Set color
    /// </summary>
#if 0 // UNUSED_LEGACY: SetColor
    void SetColor(int theR, int theG, int theB)
    {
        if (myAISContext().IsNull())
        {
            return;
        }
        Quantity_Color aCol = Quantity_Color(theR / 255., theG / 255., theB / 255., Quantity_TOC_RGB);
        for (; myAISContext()->MoreSelected(); myAISContext()->NextSelected())
        {
            myAISContext()->SetColor(myAISContext()->SelectedInteractive(), aCol, Standard_False);
        }
        //myAISContext()->SetColor(myAISContext()->SelectedInteractive(), aCol, Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: SetColor

    /// <summary>
    ///Get object color red
    /// </summary>
#if 0 // UNUSED_LEGACY: GetObjColR
    int GetObjColR(void)
    {
        int aRed, aGreen, aBlue;
        ObjectColor(aRed, aGreen, aBlue);
        return aRed;
    }
#endif // UNUSED_LEGACY: GetObjColR

    /// <summary>
    ///Get object color green
    /// </summary>
#if 0 // UNUSED_LEGACY: GetObjColG
    int GetObjColG(void)
    {
        int aRed, aGreen, aBlue;
        ObjectColor(aRed, aGreen, aBlue);
        return aGreen;
    }
#endif // UNUSED_LEGACY: GetObjColG

    /// <summary>
    ///Get object color R/G/B
    /// </summary>
#if 0 // UNUSED_LEGACY: ObjectColor
    void ObjectColor(int& theRed, int& theGreen, int& theBlue)
    {
        if (myAISContext().IsNull())
        {
            return;
        }
        theRed = 255;
        theGreen = 255;
        theBlue = 255;
        Handle(AIS_InteractiveObject) aCurrent;
        myAISContext()->InitSelected();
        if (!myAISContext()->MoreSelected())
        {
            return;
        }
        aCurrent = myAISContext()->SelectedInteractive();
        if (aCurrent->HasColor())
        {
            Quantity_Color anObjCol;
            myAISContext()->Color(aCurrent, anObjCol);
            Standard_Real r1, r2, r3;
            anObjCol.Values(r1, r2, r3, Quantity_TOC_RGB);
            theRed = (int)r1 * 255;
            theGreen = (int)r2 * 255;
            theBlue = (int)r3 * 255;
        }
    }
#endif // UNUSED_LEGACY: ObjectColor

    /// <summary>
    ///Get object color blue
    /// </summary>
#if 0 // UNUSED_LEGACY: GetObjColB
    int GetObjColB(void)
    {
        int aRed, aGreen, aBlue;
        ObjectColor(aRed, aGreen, aBlue);
        return aBlue;
    }
#endif // UNUSED_LEGACY: GetObjColB

    /// <summary>
    ///Set background color R/G/B
    /// </summary>
#if 0 // UNUSED_LEGACY: SetBackgroundColor
    void SetBackgroundColor(int theRed, int theGreen, int theBlue)
    {
        if (!myView().IsNull())
        {
            myView()->SetBackgroundColor(Quantity_TOC_RGB, theRed / 255., theGreen / 255., theBlue / 255.);
        }
    }
#endif // UNUSED_LEGACY: SetBackgroundColor

    /// <summary>
    ///Erase objects 国延林修改 2026.6
    /// </summary>
#if 0 // UNUSED_LEGACY: EraseObjects
    void EraseObjects(void)
    {
        if (myAISContext().IsNull()) return;

        // 1. 先把所有被选中的物体提取到一个安全的 std::vector 中
        std::vector<Handle(AIS_InteractiveObject)> objectsToDelete;
        for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
        {
            objectsToDelete.push_back(myAISContext()->SelectedInteractive());
        }

        if (objectsToDelete.empty()) return; // 没选中东西就直接退出

        // 让 OCC 底层的选择链表和这些即将被销毁的物体脱离关系
        myAISContext()->ClearSelected(Standard_False);
        for (size_t i = 0; i < objectsToDelete.size(); ++i)
        {
            Handle(AIS_InteractiveObject) obj = objectsToDelete[i];
            for (auto it = GlobalShapeMap.begin(); it != GlobalShapeMap.end(); )
            {
                if (it->second == obj)
                {
                    it = GlobalShapeMap.erase(it);
                    break;
                }
                else
                {
                    ++it;
                }
            }

            // 彻底从引擎中抹除
            myAISContext()->Remove(obj, Standard_False);
        }

        // 4. 刷新画面
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: EraseObjects

    /// <summary>
    ///Get version
    /// </summary>
#if 0 // UNUSED_LEGACY: GetOCCVersion
    float GetOCCVersion(void)
    {
        return (float)OCC_VERSION;
    }
#endif // UNUSED_LEGACY: GetOCCVersion

    /// <summary>
    ///set material
    /// </summary>
#if 0 // UNUSED_LEGACY: SetMaterial
    void SetMaterial(int theMaterial)
    {
        if (myAISContext().IsNull())
        {
            return;
        }
        for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
        {
            myAISContext()->SetMaterial(myAISContext()->SelectedInteractive(), (Graphic3d_NameOfMaterial)theMaterial, Standard_False);
        }
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: SetMaterial

    /// <summary>
    ///set transparency
    /// </summary>
#if 0 // UNUSED_LEGACY: SetTransparency
    void SetTransparency(int theTrans)
    {
        if (myAISContext().IsNull())
        {
            return;
        }
        for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
        {
            myAISContext()->SetTransparency(myAISContext()->SelectedInteractive(), ((Standard_Real)theTrans) / 10.0, Standard_False);
        }
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: SetTransparency

    /// <summary>
    ///Return true if object is selected
    /// </summary>
#if 0 // UNUSED_LEGACY: IsObjectSelected
    bool IsObjectSelected(void)
    {
        if (myAISContext().IsNull())
        {
            return false;
        }
        myAISContext()->InitSelected();
        return myAISContext()->MoreSelected() != Standard_False;
    }
#endif // UNUSED_LEGACY: IsObjectSelected

    /// <summary>
    ///Return display mode
    /// </summary>
#if 0 // UNUSED_LEGACY: DisplayMode
    int DisplayMode(void)
    {
        if (myAISContext().IsNull())
        {
            return -1;
        }
        int aMode = -1;
        bool OneOrMoreInShading = false;
        bool OneOrMoreInWireframe = false;
        for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
        {
            if (myAISContext()->IsDisplayed(myAISContext()->SelectedInteractive(), 1))
            {
                OneOrMoreInShading = true;
            }
            if (myAISContext()->IsDisplayed(myAISContext()->SelectedInteractive(), 0))
            {
                OneOrMoreInWireframe = true;
            }
        }
        if (OneOrMoreInShading && OneOrMoreInWireframe)
        {
            aMode = 10;
        }
        else if (OneOrMoreInShading)
        {
            aMode = 1;
        }
        else if (OneOrMoreInWireframe)
        {
            aMode = 0;
        }

        return aMode;
    }
#endif // UNUSED_LEGACY: DisplayMode

    /// <summary>
    ///Create new view
    /// </summary>
    /// <param name="theWnd">System.IntPtr that contains the window handle (HWND) of the control</param>
#if 0 // UNUSED_LEGACY: CreateNewView
    void CreateNewView(System::IntPtr theWnd)
    {
        if (myAISContext().IsNull())
        {
            return;
        }
        myView() = myAISContext()->CurrentViewer()->CreateView();
        if (myGraphicDriver().IsNull())
        {
            myGraphicDriver() = new OpenGl_GraphicDriver(Handle(Aspect_DisplayConnection)());
        }
        Handle(WNT_Window) aWNTWindow = new WNT_Window(reinterpret_cast<HWND> (theWnd.ToPointer()));
        myView()->SetWindow(aWNTWindow);
        Standard_Integer w = 100, h = 100;
        aWNTWindow->Size(w, h);
        if (!aWNTWindow->IsMapped())
        {
            aWNTWindow->Map();
        }
    }
#endif // UNUSED_LEGACY: CreateNewView

    /// <summary>
    ///Set AISContext
    /// </summary>
#if 0 // UNUSED_LEGACY: SetAISContext
    bool SetAISContext(OCCTProxy^ theViewer)
    {
        this->myAISContext() = theViewer->GetContext();
        if (myAISContext().IsNull())
        {
            return false;
        }
        return true;
    }
#endif // UNUSED_LEGACY: SetAISContext

    /// <summary>
    ///Get AISContext
    /// </summary>
#if 0 // UNUSED_LEGACY: GetContext
    Handle(AIS_InteractiveContext) GetContext(void)
    {
        return myAISContext();
    }
#endif // UNUSED_LEGACY: GetContext
    // ============================================================
    // OCC 显示质量增强：全局 Drawer 参数
    // ============================================================
    void ApplyDrawerHighQuality(const Handle(Prs3d_Drawer)& theDrawer)
    {
        if (theDrawer.IsNull())
            return;

        // 曲线、边、面显示精度。
        // 数值越小，曲线越圆滑，但显示和重绘会更慢。
        theDrawer->SetDeviationCoefficient(0.0005);
        theDrawer->SetDeviationAngle(0.03);

        // 面边界显示
        theDrawer->SetFaceBoundaryDraw(Standard_True);
        theDrawer->SetFaceBoundaryAspect(
            new Prs3d_LineAspect(
                Quantity_NOC_BLACK,
                Aspect_TOL_SOLID,
                0.8));

        // 线框显示宽度
        if (!theDrawer->LineAspect().IsNull())
        {
            theDrawer->LineAspect()->SetWidth(1.4);
        }
        else
        {
            theDrawer->SetLineAspect(
                new Prs3d_LineAspect(
                    Quantity_NOC_WHITE,
                    Aspect_TOL_SOLID,
                    1.4));
        }

        if (!theDrawer->WireAspect().IsNull())
        {
            theDrawer->WireAspect()->SetWidth(1.2);
        }
        else
        {
            theDrawer->SetWireAspect(
                new Prs3d_LineAspect(
                    Quantity_NOC_WHITE,
                    Aspect_TOL_SOLID,
                    1.2));
        }
    }


    // ============================================================
    // OCC 显示质量增强：单个 AIS 对象
    // ============================================================
    void ApplyAISObjectHighQuality(const Handle(AIS_InteractiveObject)& theObj)
    {
        if (theObj.IsNull())
            return;

        Handle(Prs3d_Drawer) aDrawer = theObj->Attributes();

        if (!aDrawer.IsNull())
        {
            ApplyDrawerHighQuality(aDrawer);
        }
    }


    // ============================================================
    // OCC 显示质量增强：根据模型尺寸计算显示网格精度
    // ============================================================
    Standard_Real ComputeDisplayMeshDeflection(const TopoDS_Shape& theShape)
    {
        if (theShape.IsNull())
            return 0.01;

        Bnd_Box aBox;
        BRepBndLib::Add(theShape, aBox);

        if (aBox.IsVoid())
            return 0.01;

        Standard_Real xmin = 0.0, ymin = 0.0, zmin = 0.0;
        Standard_Real xmax = 0.0, ymax = 0.0, zmax = 0.0;

        aBox.Get(xmin, ymin, zmin, xmax, ymax, zmax);

        Standard_Real dx = xmax - xmin;
        Standard_Real dy = ymax - ymin;
        Standard_Real dz = zmax - zmin;

        Standard_Real diag = std::sqrt(dx * dx + dy * dy + dz * dz);

        if (diag <= Precision::Confusion())
            return 0.01;

        // 相对模型尺寸的显示精度。
        // 0.00035 比 OCC 默认显示细很多。
        Standard_Real deflection = diag * 0.00035;

        // 下限防止过细导致卡死；上限防止大模型显示太粗。
        if (deflection < 0.003)
            deflection = 0.003;

        if (deflection > 0.20)
            deflection = 0.20;

        return deflection;
    }


    // ============================================================
    // OCC 显示质量增强：创建高质量 AIS_Shape
    // 以后你新显示 Shape 时，优先用这个函数创建 AIS_Shape。
    // ============================================================
    Handle(AIS_Shape) MakeHighQualityAISShape(const TopoDS_Shape& theShape)
    {
        if (theShape.IsNull())
            return NULL;

        TopoDS_Shape aShape = theShape;

        // 只影响显示三角网格，不改变真实几何。
        try
        {
            Standard_Real linDeflection = ComputeDisplayMeshDeflection(aShape);
            Standard_Real angDeflection = 0.04;

            BRepMesh_IncrementalMesh aMesher(
                aShape,
                linDeflection,
                Standard_False,
                angDeflection,
                Standard_True);
        }
        catch (...)
        {
            // 某些纯线框或异常 Shape 可能无法网格化，忽略即可。
        }

        Handle(AIS_Shape) aisShape = new AIS_Shape(aShape);

        ApplyAISObjectHighQuality(aisShape);

        return aisShape;
    }


    // ============================================================
    // OCC 显示质量增强：遍历当前所有已显示对象并重算显示
    // ============================================================
    void RefreshAllAISObjectsHighQuality()
    {
        if (myAISContext().IsNull())
            return;

        AIS_ListOfInteractive displayedObjects;
        myAISContext()->DisplayedObjects(displayedObjects);

        for (AIS_ListIteratorOfListOfInteractive it(displayedObjects); it.More(); it.Next())
        {
            Handle(AIS_InteractiveObject) obj = it.Value();

            if (obj.IsNull())
                continue;

            ApplyAISObjectHighQuality(obj);

            myAISContext()->Redisplay(obj, Standard_False);
        }

        // 你的 GlobalShapeMap 里也存了很多对象，这里再扫一遍，避免有些对象没被 DisplayedObjects 拿到。
        for (auto it = GlobalShapeMap.begin(); it != GlobalShapeMap.end(); ++it)
        {
            Handle(AIS_InteractiveObject) obj = it->second;

            if (obj.IsNull())
                continue;

            ApplyAISObjectHighQuality(obj);

            if (myAISContext()->IsDisplayed(obj))
                myAISContext()->Redisplay(obj, Standard_False);
        }

        myAISContext()->UpdateCurrentViewer();

        if (!myView().IsNull())
            myView()->Redraw();
    }


    // ============================================================
    // OCC 显示质量增强：总入口
    // 初始化 Viewer 后调用一次即可。
    // 后续导入大模型或生成很多曲线后，也可以再调用一次。
    // ============================================================
    void ApplyOCCTHighQualityDisplay()
    {
        if (myAISContext().IsNull())
            return;

        // 1. 设置全局 Drawer。之后新建 AIS_Shape 会默认吃到这些参数。
        Handle(Prs3d_Drawer) defaultDrawer = myAISContext()->DefaultDrawer();
        ApplyDrawerHighQuality(defaultDrawer);

        // 2. 视口抗锯齿。
        if (!myView().IsNull())
        {
#if OCC_VERSION_HEX >= 0x070400
            Graphic3d_RenderingParams& params = myView()->ChangeRenderingParams();

            params.IsAntialiasingEnabled = Standard_True;

            // 4 或 8 都可以。8 更平滑，但显卡压力更大。
            params.NbMsaaSamples = 8;
#endif

            myView()->SetComputedMode(Standard_False);

            // 深度裁剪重新适配，避免线框或 2D 曲线被裁掉。
            myView()->ZFitAll();
        }

        // 3. 已经显示的对象也重新套一遍高质量参数。
        RefreshAllAISObjectsHighQuality();
    }
#pragma endregion // 01_Viewer_Initialization_Interaction_And_DisplayQuality













// OLD_REGION: #pragma endregion
public:
// OLD_REGION: #pragma region 导入几何文件模型函数，杨宇昂添加，2024.5
#pragma region 90_Disabled_Legacy_Import
#if 0 // UNUSED_LEGACY: ImportBrepManaged
    bool ImportBrep(System::String^ theFileName)
    {
        return ImportBrep(toAsciiString(theFileName));
    }
#endif // UNUSED_LEGACY: ImportBrepManaged

    /// <summary>
    ///Import BRep file
    /// </summary>
    /// <param name="theFileName">Name of import file</param>
#if 0 // UNUSED_LEGACY: ImportBrepNative
    bool ImportBrep(const TCollection_AsciiString& theFileName)
    {
        TopoDS_Shape aShape;
        BRep_Builder aBuilder;
        Standard_Boolean isResult = BRepTools::Read(aShape, theFileName.ToCString(), aBuilder);
        if (!isResult)
        {
            return false;
        }

        myAISContext()->Display(new AIS_Shape(aShape), Standard_True);
        return true;
    }
#endif // UNUSED_LEGACY: ImportBrepNative


    /// <summary>
///导入Step文件，杨宇昂添加，2024.5
/// </summary>
/// <param name="theFileName">Name of import file</param>
//void ImportStep(const TCollection_AsciiString& theFileName)
#if 0 // UNUSED_LEGACY: ImportStep
    array< Double^ >^ ImportStep(System::String^ theFileName)
    {
        const TCollection_AsciiString& theFileName_Asc = toAsciiString(theFileName);
        STEPControl_Reader aReader;
        STEPCAFControl_Reader reader;
        //mySTEPCAFControl_Reader()->SetColorMode(true);
        array< Double^ >^ Point = gcnew array< Double^>(99999);
        IFSelect_ReturnStatus aStatus = aReader.ReadFile(theFileName_Asc.ToCString());
        if (aStatus == IFSelect_RetDone)
        {
            bool isFailsonly = false;
            aReader.PrintCheckLoad(isFailsonly, IFSelect_ItemsByEntity);
            int aNbRoot = aReader.NbRootsForTransfer();
            aReader.PrintCheckTransfer(isFailsonly, IFSelect_ItemsByEntity);
            for (Standard_Integer n = 1; n <= aNbRoot; n++)
            {
                Standard_Boolean ok = aReader.TransferRoot(n);
                int aNbShap = aReader.NbShapes();
                if (aNbShap > 0)
                {
                    for (int p = 1; p <= aNbShap; p++)
                    {
                        TopoDS_Shape aShape = aReader.Shape(p);
                        Handle(AIS_Shape) aisShape = MakeHighQualityAISShape(aShape);
                        myAISContext()->Display(aisShape, Standard_False);
                        TopTools_IndexedMapOfShape aEdgeMap;
                        TopExp::MapShapes(aShape, TopAbs_EDGE, aEdgeMap);
                        int i = 0;

                        for (int l = 1; l <= aEdgeMap.Extent(); l++)
                        {
                            const TopoDS_Edge& aEdge = TopoDS::Edge(aEdgeMap(l));
                            double flag = 0;
                            flag = JudgeTopoEdgeType(aEdge);
                            const TopoDS_Vertex& aFirstVertex = TopExp::FirstVertex(aEdge);
                            const TopoDS_Vertex& aLastVertex = TopExp::LastVertex(aEdge);
                            if (flag == 1)
                            {
                                gp_Pnt aPntFirst = BRep_Tool::Pnt(aFirstVertex);
                                gp_Pnt aPntLast = BRep_Tool::Pnt(aLastVertex);
                                Point[i] = flag;
                                i = i + 1;
                                Point[i] = aPntFirst.X();
                                Point[i + 1] = aPntFirst.Y();
                                Point[i + 2] = aPntFirst.Z();
                                Point[i + 3] = aPntLast.X();
                                Point[i + 4] = aPntLast.Y();
                                Point[i + 5] = aPntLast.Z();
                                i = i + 6;
                            }
                            if (flag == 2)
                            {
                                double first = 0.0;
                                double last = 0.0;
                                //Handle(Geom_TrimmedCurve)
                                Handle(Geom_Curve) handleGeomCurve = BRep_Tool::Curve(aEdge, first, last);//Edge先转化为Curve
                                Handle(Geom_TrimmedCurve) oTrimCircle = Handle(Geom_TrimmedCurve)::DownCast(handleGeomCurve);
                                Handle(Geom_Circle) PossibleCircle = Handle(Geom_Circle)::DownCast(handleGeomCurve);//Curve转化为Circle
                                //Handle(GC_MakeArcOfCircle) arcc = Handle(GC_MakeArcOfCircle)::DownCast(PossibleCircle);
                                Standard_Real U;
                                gp_Pnt P;
                                gp_Vec V1;
                                PossibleCircle->D1(U, P, V1);
                                double bb1 = P.X();
                                double bb2 = P.Y();
                                double bb3 = P.Z();
                                double AA1 = V1.X();
                                double AA2 = V1.Y();
                                double AA3 = V1.Z();
                                Standard_Real R = PossibleCircle->Radius();//获取半径
                                gp_Pnt CircleCentre = PossibleCircle->Location();//获取圆心
                                gp_Pnt aPntFirst = BRep_Tool::Pnt(aFirstVertex);
                                gp_Pnt aPntLast = BRep_Tool::Pnt(aLastVertex);
                                //Standard_Real Vec1 = handleGeomCurve->FirstParameter();
                                //Standard_Real Vec2 = handleGeomCurve->LastParameter();
                                //gp_Ax2 AX = PossibleCircle->Position();

                                //起点终点坐标
                                double PointAx = aPntFirst.X();
                                double PointAy = aPntFirst.Y();
                                double PointAz = aPntFirst.Z();
                                double PointBx = aPntLast.X();
                                double PointBy = aPntLast.Y();
                                double PointBz = aPntLast.Z();
                                //圆心坐标
                                double PointOx = CircleCentre.X();
                                double PointOy = CircleCentre.Y();
                                double PointOz = CircleCentre.Z();
                                //起始角终止角
                                double atan_PointA = atan2((PointAy - PointOy), (PointAx - PointOx));
                                double atan_Pointb = atan2((PointBy - PointOy), (PointBx - PointOx));
                                double AnglePointA = atan_PointA * 180.0 / std::acos(-1); // 使用acos(-1)获取π的值  
                                double AnglePointB = atan_Pointb * 180.0 / std::acos(-1);
                                double Direction = 1;
                                //if (Vec1 > Vec2)//起点大于终点，顺时针
                                //{
                                //    Direction = 1;
                                //}
                                //if (Vec1 < Vec2)
                                //{
                                //    Direction = 2;//起点小于终点，逆时针
                                //}
                                if (AnglePointA < 0)
                                {
                                    AnglePointA = AnglePointA + 360;
                                }
                                if (AnglePointB < 0)
                                {
                                    AnglePointB = AnglePointB + 360;
                                }
                                Point[i] = flag;
                                i = i + 1;
                                Point[i] = PointAx;
                                Point[i + 1] = PointAy;
                                Point[i + 2] = PointAz;
                                Point[i + 3] = PointBx;
                                Point[i + 4] = PointBy;
                                Point[i + 5] = PointBz;
                                Point[i + 6] = CircleCentre.X();
                                Point[i + 7] = CircleCentre.Y();
                                Point[i + 8] = CircleCentre.Z();
                                Point[i + 9] = AnglePointA;
                                Point[i + 10] = AnglePointB;
                                Point[i + 11] = R;
                                Point[i + 12] = Direction;
                                i = i + 13;
                            }
                            if (flag == 3)
                            {
                                Point[i] = flag;
                                i = i + 1;
                                double first = 0.0;
                                double last = 0.0;
                                Handle(Geom_Curve) handleGeomCurve = BRep_Tool::Curve(aEdge, first, last);//Edge先转化为Curve
                                Handle(Geom_BoundedCurve) TrimmedCurve = Handle(Geom_BoundedCurve)::DownCast(handleGeomCurve);
                                Handle(Geom_BSplineCurve) NurbsCurve = Handle(Geom_BSplineCurve)::DownCast(TrimmedCurve);//Curve转化为B样条曲线
                                double NumberOfPoles = NurbsCurve->NbPoles();//控制点个数
                                gp_Pnt PonintStar = NurbsCurve->StartPoint();//起始点
                                gp_Pnt PonintEnd = NurbsCurve->EndPoint();//终止点
                                Point[i] = PonintStar.X();
                                Point[i + 1] = PonintStar.Y();
                                Point[i + 2] = PonintStar.Z();
                                i = i + 3;
                                Point[i] = PonintEnd.X();
                                Point[i + 1] = PonintEnd.Y();
                                Point[i + 2] = PonintEnd.Z();
                                i = i + 3;
                                Point[i] = NumberOfPoles;
                                i = i + 1;
                                double Degree = NurbsCurve->Degree();//基函数阶数
                                Point[i] = Degree;
                                i = i + 1;
                                for (size_t j = 0; j < NumberOfPoles; j++)
                                {
                                    gp_Pnt ControlPoint = NurbsCurve->Pole(j + 1);
                                    Point[i] = ControlPoint.X();
                                    Point[i + 1] = ControlPoint.Y();
                                    Point[i + 2] = ControlPoint.Z();
                                    i = i + 3;
                                }//存储控制点
                                double NumberOfKnots = NurbsCurve->NbKnots();
                                Point[i] = NumberOfKnots;
                                i = i + 1;
                                TColStd_Array1OfReal Knots = NurbsCurve->Knots();//节点矢量
                                for (size_t j = 0; j < Knots.Length(); j++)
                                {
                                    double Kont = Knots[j + 1];
                                    Point[i] = Kont;
                                    i++;
                                }//存储节点矢量
                                for (size_t j = 0; j < NumberOfPoles; j++)
                                {
                                    double Weight = NurbsCurve->Weight(j + 1);
                                    Point[i] = Weight;
                                    i++;
                                }//存储权因子
                            }
                        }

                    }
                    myAISContext()->UpdateCurrentViewer();
                }
            }
        }

        return Point;
    }
#endif // UNUSED_LEGACY: ImportStep
#if 0 // UNUSED_LEGACY: ImportStep2
    bool ImportStep2(System::String^ theFileName)//失败
    {
        STEPControl_Reader aReader;
        const TCollection_AsciiString& theFileName_Asc = toAsciiString(theFileName);
        int aStatus = aReader.ReadFile(theFileName_Asc.ToCString());

        if (aStatus == IFSelect_RetDone)
        {
            aReader.TransferRoots();
            TopoDS_Shape aShape = aReader.OneShape();
            //TopoDS_Shape aShape = aReader.Shape(p);
            TopTools_IndexedMapOfShape aEdgeMap;
            TopExp::MapShapes(aShape, TopAbs_EDGE, aEdgeMap);
            for (int i = 1; i <= aEdgeMap.Extent(); ++i)
            {
                const TopoDS_Shape& shape = aReader.Shape(i);
                const TopoDS_Face& face = TopoDS::Face(shape);
                Handle(AIS_Shape) aisstep = new AIS_Shape(face);
                aisstep->SetColor(static_cast<Quantity_NameOfColor>((i + 40) % 400));
                myAISContext()->Display(aisstep, Standard_True);
            }
        }
        else
        {
            return false;
        }

        myAISContext()->UpdateCurrentViewer();
        return true;
    }
#endif // UNUSED_LEGACY: ImportStep2
#if 0 // UNUSED_LEGACY: ImportStep3
    bool ImportStep3(System::String^ theFileName)//失败
    {
        STEPControl_Reader aReader;
        const TCollection_AsciiString& theFileName_Asc = toAsciiString(theFileName);
        int aStatus = aReader.ReadFile(theFileName_Asc.ToCString());

        if (aStatus == IFSelect_RetDone)
        {
            aReader.TransferRoots();
            TopoDS_Shape aShape = aReader.OneShape();
            Handle(AIS_Shape) aisShpae = new AIS_Shape(aShape);
            aisShpae->SetDisplayMode(AIS_Shaded);
            aisShpae->SetColor(Quantity_NOC_RED);
            //aisShpae->SetMaterial(Graphic3d_NOM_PLASTIC);
            aisShpae->Attributes()->SetFaceBoundaryDraw(true);
            aisShpae->Attributes()->SetFaceBoundaryAspect(new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, 1.));
            myAISContext()->Display(aisShpae, true);
        }
        else
        {
            return false;
        }

        myAISContext()->UpdateCurrentViewer();
        return true;
    }
#endif // UNUSED_LEGACY: ImportStep3
#if 0 // UNUSED_LEGACY: ImportStep4
    void ImportStep4(System::String^ theFileName)//导入装配和颜色
    {
        //创建文档
        Handle(TDocStd_Document) doc;
        XCAFApp_Application::GetApplication()->NewDocument("MDTV-XCAF", doc);
        const TCollection_AsciiString& theFileName_Asc = toAsciiString(theFileName);
        //装载文件（.step）
        STEPCAFControl_Reader myReader;
        myReader.ReadFile(theFileName_Asc.ToCString());
        myReader.SetColorMode(true);
        myReader.SetNameMode(true);
        myReader.SetLayerMode(true);
        myReader.Transfer(doc);
        TDF_Label mainLabel = doc->Main();
        //要从XDE结构获取被视为程序集的节点，可以使用节点的Label（shape）
        Handle(XCAFDoc_ShapeTool) myShapeTool = XCAFDoc_DocumentTool::ShapeTool(mainLabel);
        //查询、编辑或初始化文档以处理XCAF的颜色(OCCT不再使用此方法渲染模型)
        Handle(XCAFDoc_ColorTool) myColors = XCAFDoc_DocumentTool::ColorTool(mainLabel);
        TDF_LabelSequence FreeShape;
        myShapeTool->GetFreeShapes(FreeShape);//使用GetFreeShapes解决装配体和组件显示错乱问题
        myShapeTool->GetShapes(FreeShape);  //获取装配体和组件对应名称（不适用）
        int Roots = FreeShape.Length();
        for (int index = 1; index <= Roots; index++)
        {
            TDF_Label label = FreeShape.Value(index);

            //用XCAFPrs_AISObject 来显示
            Handle(XCAFPrs_AISObject) displayedShape = new XCAFPrs_AISObject(label);
            myAISContext()->Display(displayedShape, true);
        }
    }
#endif // UNUSED_LEGACY: ImportStep4
    /// <summary>
    ///Import Iges file
    /// </summary>
    /// <param name="theFileName">Name of import file</param>
#if 0 // UNUSED_LEGACY: ImportIges
    bool ImportIges(const TCollection_AsciiString& theFileName)
    {
        IGESControl_Reader aReader;
        int aStatus = aReader.ReadFile(theFileName.ToCString());

        if (aStatus == IFSelect_RetDone)
        {
            aReader.TransferRoots();
            TopoDS_Shape aShape = aReader.OneShape();
            Handle(AIS_Shape) aisShape = MakeHighQualityAISShape(aShape);
            myAISContext()->Display(aisShape, Standard_False);
        }
        else
        {
            return false;
        }

        myAISContext()->UpdateCurrentViewer();
        return true;
    }
#endif // UNUSED_LEGACY: ImportIges
#pragma endregion // 90_Disabled_Legacy_Import
// OLD_REGION: #pragma endregion
// OLD_REGION: #pragma region 导出几何文件模型函数，杨宇昂，2024.5
    /// <summary>
    ///Export BRep file
    /// </summary>
    /// <param name="theFileName">Name of export file</param>
#pragma region 02_STEP_XCAF_Export_And_Disabled_Legacy_Export
#if 0 // UNUSED_LEGACY: ExportBRep
    bool ExportBRep(const TCollection_AsciiString& theFileName)
    {
        myAISContext()->InitSelected();
        if (!myAISContext()->MoreSelected())
        {
            return false;
        }

        Handle(AIS_InteractiveObject) anIO = myAISContext()->SelectedInteractive();
        Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(anIO);
        return BRepTools::Write(anIS->Shape(), theFileName.ToCString()) != Standard_False;
    }
#endif // UNUSED_LEGACY: ExportBRep

    /// <summary>
    ///Export Step file
    /// </summary>
    /// <param name="theFileName">Name of export file</param>
#if 0 // UNUSED_LEGACY: ExportStep
    bool ExportStep(System::String^ theFileName)
    {
        const TCollection_AsciiString& theFileName_Asc = toAsciiString(theFileName);
        STEPControl_StepModelType aType = STEPControl_AsIs;
        IFSelect_ReturnStatus aStatus;
        STEPControl_Writer aWriter;
        for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
        {
            Handle(AIS_InteractiveObject) anIO = myAISContext()->SelectedInteractive();
            Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(anIO);
            TopoDS_Shape aShape = anIS->Shape();
            aStatus = aWriter.Transfer(aShape, aType);
            if (aStatus != IFSelect_RetDone)
            {
                return false;
            }
        }
        aStatus = aWriter.Write(theFileName_Asc.ToCString());
        if (aStatus != IFSelect_RetDone)
        {
            return false;
        }
        return true;
    }
#endif // UNUSED_LEGACY: ExportStep

#if 0 // UNUSED_LEGACY: ExportStep2
    bool ExportStep2(System::String^ theFileName)
    {
        AIS_ListOfInteractive displayedObjects;
        myAISContext()->DisplayedObjects(displayedObjects);
        const TCollection_AsciiString& theFileName_Asc = toAsciiString(theFileName);
        STEPControl_StepModelType aType = STEPControl_AsIs;
        IFSelect_ReturnStatus aStatus;
        STEPControl_Writer aWriter;
        for (AIS_ListIteratorOfListOfInteractive it(displayedObjects); it.More(); it.Next()) {
            const Handle(AIS_InteractiveObject)& obj = it.Value();
            myAISContext()->AddOrRemoveSelected(obj, Standard_False); // 添加选中
        }
        for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
        {
            Handle(AIS_InteractiveObject) anIO = myAISContext()->SelectedInteractive();
            Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(anIO);
            TopoDS_Shape aShape = anIS->Shape();
            aStatus = aWriter.Transfer(aShape, aType);
            if (aStatus != IFSelect_RetDone)
            {
                return false;
            }
        }
        aStatus = aWriter.Write(theFileName_Asc.ToCString());
        if (aStatus != IFSelect_RetDone)
        {
            return false;
        }
        return true;
    }
#endif // UNUSED_LEGACY: ExportStep2
    //将界面显示的内容导出为STP文件
    bool ExportVisibleToSTEP(System::String^ filePath,array<System::String^>^ componentNames,array<array<int>^>^ componentShapeIds)
    {
        if (myAISContext().IsNull()
            || GlobalShapeMap.empty()
            || componentNames == nullptr
            || componentShapeIds == nullptr
            || componentNames->Length == 0
            || componentNames->Length != componentShapeIds->Length)
        {
            return false;
        }

        // ----------------------------------------------------------
        // 1. 创建 XCAF 文档
        // ----------------------------------------------------------
        Handle(TDocStd_Document) document;

        XCAFApp_Application::GetApplication()->NewDocument(
            "MDTV-XCAF",
            document);

        if (document.IsNull())
        {
            return false;
        }

        Handle(XCAFDoc_ShapeTool) shapeTool =
            XCAFDoc_DocumentTool::ShapeTool(document->Main());

        if (shapeTool.IsNull())
        {
            return false;
        }

        // ----------------------------------------------------------
        // 2. 创建整个轮胎模型的根装配
        // ----------------------------------------------------------
        TDF_Label rootAssemblyLabel =
            shapeTool->NewShape();

        TDataStd_Name::Set(
            rootAssemblyLabel,
            TCollection_ExtendedString("Tire_Assembly"));

        int exportedComponentCount = 0;

        // ----------------------------------------------------------
        // 3. 每个软件 Component 创建一个 STEP Part
        // ----------------------------------------------------------
        for (int componentIndex = 0;
            componentIndex < componentShapeIds->Length;
            ++componentIndex)
        {
            array<int>^ shapeIds =
                componentShapeIds[componentIndex];

            if (shapeIds == nullptr || shapeIds->Length == 0)
            {
                continue;
            }

            // 一个 Component 对应一个 Compound
            BRep_Builder compoundBuilder;
            TopoDS_Compound componentCompound;

            compoundBuilder.MakeCompound(componentCompound);

            int exportedShapeCount = 0;

            // 将当前 Component 中的所有面放入 Compound
            for each (int shapeId in shapeIds)
            {
                auto shapeIterator =
                    GlobalShapeMap.find(shapeId);

                if (shapeIterator == GlobalShapeMap.end())
                {
                    continue;
                }

                Handle(AIS_InteractiveObject) aisObject =
                    shapeIterator->second;

                if (aisObject.IsNull())
                {
                    continue;
                }

                // 保持原有逻辑：隐藏的面不导出
                if (!myAISContext()->IsDisplayed(aisObject))
                {
                    continue;
                }

                Handle(AIS_Shape) aisShape =
                    Handle(AIS_Shape)::DownCast(aisObject);

                if (aisShape.IsNull())
                {
                    continue;
                }

                TopoDS_Shape shape = aisShape->Shape();

                if (shape.IsNull())
                {
                    continue;
                }

                compoundBuilder.Add(
                    componentCompound,
                    shape);

                ++exportedShapeCount;
            }

            // 整个 Component 为空或已隐藏时不导出
            if (exportedShapeCount == 0)
            {
                continue;
            }

            // ------------------------------------------------------
            // 4. 把 Compound 登记成一个 Part
            //
            // Standard_False 非常重要：
            // 表示 Compound 是一个 Part，而不是继续展开为子装配。
            // ------------------------------------------------------
            TDF_Label partLabel =
                shapeTool->AddShape(
                    componentCompound,
                    Standard_False);

            // 设置 Component 名称
            TCollection_AsciiString componentNameAscii =
                toAsciiString(componentNames[componentIndex]);

            TCollection_ExtendedString componentName(
                componentNameAscii.ToCString(),
                Standard_True);

            TDataStd_Name::Set(
                partLabel,
                componentName);

            // 将 Part 挂到 Tire_Assembly 根装配下面
            TDF_Label instanceLabel =
                shapeTool->AddComponent(
                    rootAssemblyLabel,
                    partLabel,
                    TopLoc_Location());

            // 同时给装配实例设置名称
            TDataStd_Name::Set(
                instanceLabel,
                componentName);

            ++exportedComponentCount;
        }

        if (exportedComponentCount == 0)
        {
            return false;
        }

        // 根据 Component 关系重新计算根装配 Compound
        shapeTool->UpdateAssemblies();

        // ----------------------------------------------------------
        // 5. 使用 STEPCAFControl_Writer 导出层级
        // ----------------------------------------------------------
        Interface_Static::SetCVal(
            "write.step.unit",
            "MM");

        // 保留你原来使用的 AP214 设置
        Interface_Static::SetIVal(
            "write.step.schema",
            5);

        STEPCAFControl_Writer writer;

        // 只要求名称和装配层级，不导出颜色、图层
        writer.SetNameMode(Standard_True);
        writer.SetColorMode(Standard_False);
        writer.SetLayerMode(Standard_False);

        if (!writer.Transfer(
            document,
            STEPControl_AsIs))
        {
            return false;
        }

        TCollection_AsciiString outputPath =
            toAsciiString(filePath);

        IFSelect_ReturnStatus writeStatus =
            writer.Write(outputPath.ToCString());

        return writeStatus == IFSelect_RetDone;
    }

    /// <summary>
    ///Export Iges file
    /// </summary>
    /// <param name="theFileName">Name of export file</param>
#if 0 // UNUSED_LEGACY: ExportIges
    bool ExportIges(const TCollection_AsciiString& theFileName)
    {
        IGESControl_Controller::Init();
        IGESControl_Writer aWriter(Interface_Static::CVal("XSTEP.iges.unit"),
            Interface_Static::IVal("XSTEP.iges.writebrep.mode"));

        for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
        {
            Handle(AIS_InteractiveObject) anIO = myAISContext()->SelectedInteractive();
            Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(anIO);
            TopoDS_Shape aShape = anIS->Shape();
            aWriter.AddShape(aShape);
        }

        aWriter.ComputeModel();
        return aWriter.Write(theFileName.ToCString()) != Standard_False;
    }
#endif // UNUSED_LEGACY: ExportIges

    /// <summary>
    ///Export Vrml file
    /// </summary>
    /// <param name="theFileName">Name of export file</param>
#if 0 // UNUSED_LEGACY: ExportVrml
    bool ExportVrml(const TCollection_AsciiString& theFileName)
    {
        TopoDS_Compound aRes;
        BRep_Builder aBuilder;
        aBuilder.MakeCompound(aRes);

        for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
        {
            Handle(AIS_InteractiveObject) anIO = myAISContext()->SelectedInteractive();
            Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(anIO);
            TopoDS_Shape aShape = anIS->Shape();
            if (aShape.IsNull())
            {
                return false;
            }

            aBuilder.Add(aRes, aShape);
        }

        VrmlAPI_Writer aWriter;
        aWriter.Write(aRes, theFileName.ToCString());

        return true;
    }
#endif // UNUSED_LEGACY: ExportVrml

    /// <summary>
    ///Export Stl file
    /// </summary>
    /// <param name="theFileName">Name of export file</param>
#if 0 // UNUSED_LEGACY: ExportStl
    bool ExportStl(const TCollection_AsciiString& theFileName)
    {
        TopoDS_Compound aComp;
        BRep_Builder aBuilder;
        aBuilder.MakeCompound(aComp);

        for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
        {
            Handle(AIS_InteractiveObject) anIO = myAISContext()->SelectedInteractive();
            Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast(anIO);
            TopoDS_Shape aShape = anIS->Shape();
            if (aShape.IsNull())
            {
                return false;
            }
            aBuilder.Add(aComp, aShape);
        }

        StlAPI_Writer aWriter;
        aWriter.Write(aComp, theFileName.ToCString());
        return true;
    }
#endif // UNUSED_LEGACY: ExportStl
#pragma endregion // 02_STEP_XCAF_Export_And_Disabled_Legacy_Export

// OLD_REGION: #pragma endregion
// OLD_REGION: #pragma region 创建几何文件模型函数，杨宇昂，2024.5
    //!创建长方体四种方法（已经全部编程完毕）
#pragma region 91_Disabled_Legacy_Solid_Primitives
#if 0 // UNUSED_LEGACY: CreateBoxFunction1
    void CreateBoxFunction1(Standard_Real Box_X, Standard_Real Box_Y, Standard_Real Box_Z)
    {
        BRepPrimAPI_MakeBox Box(Box_X, Box_Y, Box_Z);
        const TopoDS_Shape& shape = Box.Shape();
        myAISContext()->Display(new AIS_Shape(shape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateBoxFunction1
    //!长宽高确定一个长方体，中心点在0，0，0，杨宇昂添加，2024.6
#if 0 // UNUSED_LEGACY: CreateBoxFunction2
    void CreateBoxFunction2(Standard_Real Box_Corner_X, Standard_Real Box_Corner_Y, Standard_Real Box_Corner_Z, Standard_Real Box_X, Standard_Real Box_Y, Standard_Real Box_Z)
    {
        BRepPrimAPI_MakeBox Box(gp_Pnt(Box_Corner_X, Box_Corner_Y, Box_Corner_Z), Box_X, Box_Y, Box_Z);
        const TopoDS_Shape& shape = Box.Shape();
        myAISContext()->Display(new AIS_Shape(shape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateBoxFunction2
    //!长宽高确定一个长方体，中心点在Box_Corner_X，Box_Corner_Y，Box_Corner_Z，杨宇昂添加，2024.6
#if 0 // UNUSED_LEGACY: CreateBoxFunction3
    void CreateBoxFunction3(Standard_Real Point1_X, Standard_Real Point1_Y, Standard_Real Point1_Z, Standard_Real Point2_X, Standard_Real Point2_Y, Standard_Real Point2_Z)
    {
        BRepPrimAPI_MakeBox Box(gp_Pnt(Point1_X, Point1_Y, Point1_Z), gp_Pnt(Point2_X, Point2_Y, Point2_Z));
        const TopoDS_Shape& shape = Box.Shape();
        myAISContext()->Display(new AIS_Shape(shape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateBoxFunction3
    //两点确定一个长方体.，杨宇昂添加，2024.6
#if 0 // UNUSED_LEGACY: CreateBoxFunction4
    void CreateBoxFunction4(Standard_Real Box_Corner_X, Standard_Real Box_Corner_Y, Standard_Real Box_Corner_Z, Standard_Real Vertor_X, Standard_Real Vertor_Y, Standard_Real Vertor_Z, Standard_Real Box_X, Standard_Real Box_Y, Standard_Real Box_Z)
    {
        BRepPrimAPI_MakeBox Box(gp_Ax2(gp_Pnt(Box_Corner_X, Box_Corner_Y, Box_Corner_Z), gp_Dir(Vertor_X, Vertor_Y, Vertor_Z)), Box_X, Box_Y, Box_Z);
        const TopoDS_Shape& shape = Box.Shape();
        myAISContext()->Display(new AIS_Shape(shape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateBoxFunction4
    //确定一个点，确定点上的向量，再确定三个边长，杨宇昂添加，2024.6


     //!创建楔形四种方法（已经全部编程完毕）
#if 0 // UNUSED_LEGACY: CreateWedgeFunction1
    void CreateWedgeFunction1(const Standard_Real dx, const Standard_Real dy, const Standard_Real dz, const Standard_Real Itx)
    {
        BRepPrimAPI_MakeWedge Wedge(dx, dy, dz, Itx);
        const TopoDS_Shape& shape = Wedge.Shape();
        myAISContext()->Display(new AIS_Shape(shape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateWedgeFunction1
    //dx  dz确定一个平面（底面），dy是高，Itx是上平面那条dx对应的边，上平面的边是Itx和dz，杨宇昂添加，2024.6
#if 0 // UNUSED_LEGACY: CreateWedgeFunction2
    void CreateWedgeFunction2(const Standard_Real dx, const Standard_Real dy, const Standard_Real dz, const Standard_Real xmin, const Standard_Real zmin, const Standard_Real xmax, const Standard_Real zmax)
    {
        BRepPrimAPI_MakeWedge Wedge(dx, dy, dz, xmin, zmin, xmax, zmax);
        const TopoDS_Shape& shape = Wedge.Shape();
        myAISContext()->Display(new AIS_Shape(shape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateWedgeFunction2
    //dx dz确定一个平面（底面），dy是高，xmin, zmin, xmax, zmax确定上平面，杨宇昂添加，2024.6
#if 0 // UNUSED_LEGACY: CreateWedgeFunction3
    void CreateWedgeFunction3(const Standard_Real dx, const Standard_Real dy, const Standard_Real dz)
    {
        BRepPrimAPI_MakeWedge Wedge(dx, dy, dz, dx / 2, 0, dx / 2, dz);
        const TopoDS_Shape& shape = Wedge.Shape();
        myAISContext()->Display(new AIS_Shape(shape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateWedgeFunction3
    //dx dz确定一个平面（底面），dy是高，上表面是dx/2, 0, dx/2, dz确定的一条线，杨宇昂添加，2024.6
#if 0 // UNUSED_LEGACY: CreateWedgeFunction4
    void CreateWedgeFunction4(const Standard_Real dx, const Standard_Real dy, const Standard_Real dz)
    {
        BRepPrimAPI_MakeWedge Wedge(dx, dy, dz, dx / 2, dz / 2, dx / 2, dz / 2);
        const TopoDS_Shape& shape = Wedge.Shape();
        myAISContext()->Display(new AIS_Shape(shape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateWedgeFunction4
    //dx dz确定一个平面（底面），dy是高，上表面是dx / 2, dz/2, dx / 2, dz/2确定的一个点，杨宇昂添加，2024.6


     //!创建柱体两种方法（c++四种方法都可以化为这一个方法，其他的都是特例），杨宇昂添加，2024.6
#if 0 // UNUSED_LEGACY: CreateCylinderFunction1
    void CreateCylinderFunction1(double Center_X, double Center_Y, double Center_Z, double Vector_X, double Vector_Y, double Vector_Z, double r, double h, double angle)
    {
        BRepPrimAPI_MakeCylinder Cylinder(gp_Ax2(gp_Pnt(Center_X, Center_Y, Center_Z), gp_Dir(Vector_X, Vector_Y, Vector_Z)), r, h, angle);
        const TopoDS_Shape& shape = Cylinder.Shape();
        myAISContext()->Display(new AIS_Shape(shape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateCylinderFunction1
    //圆柱的圆心，方向，高度，半径，和旋转角度确定一个圆柱（对应c++方法四），杨宇昂添加，2024.6


    //!创建圆锥方法（c++四种方法都可以化为这一个方法），杨宇昂添加，2024.6
#if 0 // UNUSED_LEGACY: CreateConeFunction1
    void CreateConeFunction1(double Center_X, double Center_Y, double Center_Z, double Vector_X, double Vector_Y, double Vector_Z, double R1, double R2, double h, double angle)
    {
        BRepPrimAPI_MakeCone Cone(gp_Ax2(gp_Pnt(Center_X, Center_Y, Center_Z), gp_Dir(Vector_X, Vector_Y, Vector_Z)), R1, R2, h, angle);
        const TopoDS_Shape& shape = Cone.Shape();
        myAISContext()->Display(new AIS_Shape(shape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateConeFunction1
    //圆锥的圆心，方向，高度，半径，和旋转角度确定一个圆锥（对应c++方法四），杨宇昂添加，2024.6


    //!创建球的方法（c++其他方法都可以算作这个方法的特殊情况），杨宇昂添加，2024.6
#if 0 // UNUSED_LEGACY: CreateSphereFunction1
    void CreateSphereFunction1(double Center_X, double Center_Y, double Center_Z, double r, double angle)
    {
        BRepPrimAPI_MakeSphere Sphere(gp_Pnt(Center_X, Center_Y, Center_Z), r);
        const TopoDS_Shape& shape = Sphere.Shape();
        myAISContext()->Display(new AIS_Shape(shape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateSphereFunction1//球的球心和半径和角度,angleupward（球体上方有多少角度），angledownward（球体下方有多少角度）确定一个球
    //球的球心，方向，半径，上球面角度，下球面角度，旋转度数（对应c++最后一个方法），杨宇昂添加，2024.6


    //!创建圆环的方法（c++其他方法都可以算作这个方法的特殊情况）
#if 0 // UNUSED_LEGACY: CreateTorusFunction1
    void CreateTorusFunction1(double Center_X, double Center_Y, double Center_Z, double Vector_X, double Vector_Y, double Vector_Z, double R1, double R2)
    {
        BRepPrimAPI_MakeTorus Torus(gp_Ax2(gp_Pnt(Center_X, Center_Y, Center_Z), gp_Dir(Vector_X, Vector_Y, Vector_Z)), R1, R2);
        //BRepPrimAPI_MakeTorus Torus(gp_Ax2(gp_Pnt(Center_X, Center_Y, Center_Z), gp_Dir(Vector_X, Vector_Y, Vector_Z)), R1, R2, angle_upward, angle_downward, angle);
        const TopoDS_Shape& shape = Torus.Shape();
        myAISContext()->Display(new AIS_Shape(shape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateTorusFunction1//球的球心和半径和角度,angleupward（球体上方有多少角度），angledownward（球体下方有多少角度）确定一个球，杨宇昂添加，2024.6

     //圆环的圆环心，方向，圆环左半径，圆环右半径，上圆环角度，下圆环角度，旋转度数（对应c++最后一个方法），杨宇昂添加，2024.6
#if 0 // UNUSED_LEGACY: MakePrism
    void MakePrism(TopoDS_Shape shape_2d, double Vector_X, double Vector_Y, double Vector_Z)
    {
        BRepPrimAPI_MakePrism shape_Prism(shape_2d, gp_Vec(Vector_X, Vector_Y, Vector_Z));
        const TopoDS_Shape& shape = shape_Prism.Shape();
        myAISContext()->Display(new AIS_Shape(shape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: MakePrism

#if 0 // UNUSED_LEGACY: MakeRevol
    void MakeRevol(TopoDS_Shape shape_2d, double Center_X, double Center_Y, double Center_Z, double Vector_X, double Vector_Y, double Vector_Z, double angle)
    {
        BRepPrimAPI_MakeRevol shape_Revol(shape_2d, gp_Ax1(gp_Pnt(Center_X, Center_Y, Center_Z), gp_Dir(Vector_X, Vector_Y, Vector_Z)), angle);
        const TopoDS_Shape& shape = shape_Revol.Shape();
        myAISContext()->Display(new AIS_Shape(shape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: MakeRevol//暂时未完成
#pragma endregion // 91_Disabled_Legacy_Solid_Primitives

// OLD_REGION: #pragma endregion
// OLD_REGION: #pragma region 创建点线面，杨宇昂添加，2024.4
#pragma region 03_Basic_Geometry_And_Disabled_Legacy_Drawing
#if 0 // UNUSED_LEGACY: CreatePoint
    void CreatePoint(double Center_X, double Center_Y, double Center_Z)
    {
        //创建点并显示
        gp_Pnt aPnt(Center_X, Center_Y, Center_Z);
        //创建顶点
        TopoDS_Shape aShape = BRepBuilderAPI_MakeVertex(aPnt);
        Handle(AIS_Shape) aisShape = MakeHighQualityAISShape(aShape);
        myAISContext()->Display(aisShape, Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreatePoint

    //线创建函数改造 国延林 2026.6.18
    void CreateLine(int id, double X1, double X2, double Y1, double Y2, double Z1, double Z2)
    {
        gp_Pnt aPnt1(X1, Y1, Z1);
        gp_Pnt aPnt2(X2, Y2, Z2);
        TopoDS_Shape aShape = BRepBuilderAPI_MakeEdge(aPnt1, aPnt2);

        Handle(AIS_Shape) aisShape = new AIS_Shape(aShape);

        // 【修改点 2】：把生成的对象以 id 为钥匙锁进档案室
        GlobalShapeMap[id] = aisShape;

        myAISContext()->Display(aisShape, Standard_False);
    }

#if 0 // UNUSED_LEGACY: CreateSurface
    void CreateSurface(double X1, double Y1, double Z1, double X2, double Y2, double Z2, double X3, double Y3, double Z3, double U1, double V1, double U2, double V2)
    {
        //创建点并显示
        gp_Pnt aPnt1(X1, Y1, Z1);
        gp_Pnt aPnt2(X2, Y2, Z2);
        gp_Pnt aPnt3(X3, Y3, Z3);
        gp_Pln aPln = gce_MakePln(aPnt1, aPnt2, aPnt3);
        TopoDS_Shape aShape = BRepBuilderAPI_MakeFace(aPln, U1, U2, V1, V2);
        //显示
        Handle(AIS_Shape) aisShape = MakeHighQualityAISShape(aShape);
        myAISContext()->Display(aisShape, Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateSurface
// OLD_REGION: #pragma endregion
    //显示坐标轴，杨宇昂添加，2024.6
#if 0 // UNUSED_LEGACY: Show3dAxo
    void Show3dAxo()
    {
        myView()->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_WHITE, 0.2, V3d_ZBUFFER);
    }
#endif // UNUSED_LEGACY: Show3dAxo

    //初始化OCC，杨宇昂添加，2024.6
    void InitOCCTProxy(void)
    {
        myGraphicDriver() = NULL;
        myViewer() = NULL;
        myView() = NULL;
        myAISContext() = NULL;
    }

// OLD_REGION: #pragma region 判断是直线还是圆弧，杨宇昂添加，2024.6
#if 0 // UNUSED_LEGACY: JudgeTopoEdgeType
    int JudgeTopoEdgeType(const TopoDS_Edge& TopoEdge)
    {
        double first = 0.0;
        double last = 0.0;
        int flag = 0;
        Handle(Geom_Curve) handleGeomCurve = BRep_Tool::Curve(TopoEdge, first, last);
        if (handleGeomCurve.IsNull())
        {
            return 0;
        }
        if (handleGeomCurve->IsKind(STANDARD_TYPE(Geom_Line)))
        {
            flag = 1;
        }
        else if (handleGeomCurve->IsKind(STANDARD_TYPE(Geom_Circle)))
        {
            flag = 2;
        }
        else if (handleGeomCurve->IsKind(STANDARD_TYPE(Geom_BSplineCurve)))
        {
            flag = 3;
        }
        return flag;
    }
#endif // UNUSED_LEGACY: JudgeTopoEdgeType
// OLD_REGION: #pragma endregion
#if 0 // UNUSED_LEGACY: InterGetCoordinate
    void InterGetCoordinate(int x, int y)//获取鼠标所在位置的坐标，有问题
    {
        Standard_Real X, Y, Z;
        gp_Pln plane = gce_MakePln(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)); // XY 平面
        myView()->Convert(x, y, X, Y, Z);
        gp_Pnt ConvertedPoint(X, Y, Z);
        gp_Pnt2d ConvertedPointOnPlane = ProjLib::Project(plane, ConvertedPoint);
        gp_Pnt ResultPoint = ElSLib::Value(ConvertedPointOnPlane.X(), ConvertedPointOnPlane.Y(), plane);
        double realX = ResultPoint.X();
    }
#endif // UNUSED_LEGACY: InterGetCoordinate

// OLD_REGION: #pragma region //点击两个点创建一条直线，杨宇昂添加，2025.4
#if 0 // UNUSED_LEGACY: InterCreateLine
    void InterCreateLine()
    {
        gp_Pnt point_1;
        gp_Pnt point_2;

        //第一个点
        myAISContext()->InitSelected();
        TopoDS_Shape selectedShape_1 = myAISContext()->SelectedShape();
        if (selectedShape_1.ShapeType() == TopAbs_VERTEX)
        {
            TopoDS_Vertex vertex_1 = TopoDS::Vertex(selectedShape_1);
            point_1 = BRep_Tool::Pnt(vertex_1);
            double X = point_1.X();
            double Y = point_1.Y();
            double Z = point_1.Z();
        }
        //第二个点
        myAISContext()->NextSelected();
        TopoDS_Shape selectedShape_2 = myAISContext()->SelectedShape();
        if (selectedShape_2.ShapeType() == TopAbs_VERTEX)
        {
            TopoDS_Vertex vertex_2 = TopoDS::Vertex(selectedShape_2);
            point_2 = BRep_Tool::Pnt(vertex_2);
            double X = point_2.X();
            double Y = point_2.Y();
            double Z = point_2.Z();
        }

        myAISContext()->ClearSelected(Standard_False);

        //创建线
        TopoDS_Shape aShape = BRepBuilderAPI_MakeEdge(point_1, point_2);
        //显示
        Handle(AIS_Shape) aisShape = MakeHighQualityAISShape(aShape);
        myAISContext()->Display(aisShape, Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: InterCreateLine
// OLD_REGION: #pragma endregion
// OLD_REGION: #pragma region 布尔操作函数，杨宇昂添加，2025.4
#if 0 // UNUSED_LEGACY: AlgoAPI_Cut
    void AlgoAPI_Cut()
    {
        myAISContext()->InitSelected();
        TopoDS_Shape selectedShape_1 = myAISContext()->SelectedShape();

        myAISContext()->NextSelected();
        TopoDS_Shape selectedShape_2 = myAISContext()->SelectedShape();

        TopoDS_Shape ShapeCut = BRepAlgoAPI_Cut(selectedShape_1, selectedShape_2);

        myAISContext()->EraseAll(Standard_True);
        myAISContext()->Display(new AIS_Shape(ShapeCut), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: AlgoAPI_Cut

#if 0 // UNUSED_LEGACY: AlgoAPI_Jia
    void AlgoAPI_Jia()
    {
        myAISContext()->InitSelected();
        TopoDS_Shape selectedShape_1 = myAISContext()->SelectedShape();

        myAISContext()->NextSelected();
        TopoDS_Shape selectedShape_2 = myAISContext()->SelectedShape();

        TopoDS_Shape ShapeCut = BRepAlgoAPI_Fuse(selectedShape_1, selectedShape_2);

        myAISContext()->EraseAll(Standard_True);
        myAISContext()->Display(new AIS_Shape(ShapeCut), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: AlgoAPI_Jia

#if 0 // UNUSED_LEGACY: AlgoAPI_Jiao
    void AlgoAPI_Jiao()
    {
        myAISContext()->InitSelected();
        TopoDS_Shape selectedShape_1 = myAISContext()->SelectedShape();

        myAISContext()->NextSelected();
        TopoDS_Shape selectedShape_2 = myAISContext()->SelectedShape();

        TopoDS_Shape ShapeCut = BRepAlgoAPI_Common(selectedShape_1, selectedShape_2);

        myAISContext()->EraseAll(Standard_True);
        myAISContext()->Display(new AIS_Shape(ShapeCut), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: AlgoAPI_Jiao

#if 0 // UNUSED_LEGACY: boolerFuse
    void boolerFuse()
    {
        Handle(AIS_InteractiveObject) anIO1 = myAISContext()->SelectedInteractive();
        Handle(AIS_Shape) anIS1 = Handle(AIS_Shape)::DownCast(anIO1);
        TopoDS_Shape aShape1 = anIS1->Shape();
        Handle(AIS_InteractiveObject) anIO2 = myAISContext()->SelectedInteractive();
        Handle(AIS_Shape) anIS2 = Handle(AIS_Shape)::DownCast(anIO2);
        TopoDS_Shape aShape2 = anIS2->Shape();

        TopoDS_Shape FusedShape = BRepAlgoAPI_Fuse(aShape1, aShape2);

        myAISContext()->Erase(anIS1, Standard_True);
        myAISContext()->Erase(anIS2, Standard_True);
    }
#endif // UNUSED_LEGACY: boolerFuse
// OLD_REGION: #pragma endregion
// OLD_REGION: #pragma region 创建三角形函数，杨宇昂添加，2025.4
#if 0 // UNUSED_LEGACY: CreateTriangleXOY
    void CreateTriangleXOY(double X1, double Y1, double Z1, double X2, double Y2, double Z2, double X3, double Y3, double Z3)
    {
        gp_Pnt A(X1, Y1, Z1); // 左下
        gp_Pnt B(X2, Y2, Z2); // 左上
        gp_Pnt C(X3, Y3, Z3);  // 右上

        BRepBuilderAPI_MakePolygon polygon;
        polygon.Add(A);
        polygon.Add(B);
        polygon.Add(C);
        polygon.Close(); // 闭合线框

        TopoDS_Wire wire = polygon.Wire();
        gp_Pln plane(gp::XOY());
        BRepBuilderAPI_MakeFace faceMaker(plane, wire);
        TopoDS_Face face = faceMaker.Face();
        TopoDS_Shape myShape = face;
        myAISContext()->Display(new AIS_Shape(myShape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateTriangleXOY

#if 0 // UNUSED_LEGACY: CreateTriangleYOZ
    void CreateTriangleYOZ(double X1, double Y1, double Z1, double X2, double Y2, double Z2, double X3, double Y3, double Z3)
    {
        gp_Pnt A(X1, Y1, Z1); // 左下
        gp_Pnt B(X2, Y2, Z2); // 左上
        gp_Pnt C(X3, Y3, Z3);  // 右上

        BRepBuilderAPI_MakePolygon polygon;
        polygon.Add(A);
        polygon.Add(B);
        polygon.Add(C);
        polygon.Close(); // 闭合线框

        TopoDS_Wire wire = polygon.Wire();
        gp_Pln plane(gp::YOZ());
        BRepBuilderAPI_MakeFace faceMaker(plane, wire);
        TopoDS_Face face = faceMaker.Face();
        TopoDS_Shape myShape = face;
        myAISContext()->Display(new AIS_Shape(myShape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateTriangleYOZ

#if 0 // UNUSED_LEGACY: CreateTriangleZOX
    void CreateTriangleZOX(double X1, double Y1, double Z1, double X2, double Y2, double Z2, double X3, double Y3, double Z3)
    {
        gp_Pnt A(X1, Y1, Z1); // 左下
        gp_Pnt B(X2, Y2, Z2); // 左上
        gp_Pnt C(X3, Y3, Z3);  // 右上

        BRepBuilderAPI_MakePolygon polygon;
        polygon.Add(A);
        polygon.Add(B);
        polygon.Add(C);
        polygon.Close(); // 闭合线框

        TopoDS_Wire wire = polygon.Wire();
        gp_Pln plane(gp::ZOX());
        BRepBuilderAPI_MakeFace faceMaker(plane, wire);
        TopoDS_Face face = faceMaker.Face();
        TopoDS_Shape myShape = face;
        myAISContext()->Display(new AIS_Shape(myShape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateTriangleZOX
// OLD_REGION: #pragma endregion
// OLD_REGION: #pragma region 创建四边形函数，杨宇昂添加，2025.4
#if 0 // UNUSED_LEGACY: CreateQuadXOY
    void CreateQuadXOY(double X1, double Y1, double Z1, double X2, double Y2, double Z2, double X3, double Y3, double Z3, double X4, double Y4, double Z4)
    {
        gp_Pnt A(X1, Y1, Z1); // 左下
        gp_Pnt B(X2, Y2, Z2); // 左上
        gp_Pnt C(X3, Y3, Z3);  // 右上
        gp_Pnt D(X4, Y4, Z4);  // 右下

        BRepBuilderAPI_MakePolygon polygon;
        polygon.Add(A);
        polygon.Add(B);
        polygon.Add(C);
        polygon.Add(D);
        polygon.Close(); // 闭合线框

        TopoDS_Wire wire = polygon.Wire();
        gp_Pln plane(gp::XOY());
        BRepBuilderAPI_MakeFace faceMaker(plane, wire);
        TopoDS_Face face = faceMaker.Face();
        TopoDS_Shape myShape = face;
        myAISContext()->Display(new AIS_Shape(myShape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateQuadXOY

#if 0 // UNUSED_LEGACY: CreateQuadYOZ
    void CreateQuadYOZ(double X1, double Y1, double Z1, double X2, double Y2, double Z2, double X3, double Y3, double Z3, double X4, double Y4, double Z4)
    {
        gp_Pnt A(X1, Y1, Z1); // 左下
        gp_Pnt B(X2, Y2, Z2); // 左上
        gp_Pnt C(X3, Y3, Z3);  // 右上
        gp_Pnt D(X4, Y4, Z4);  // 右下

        BRepBuilderAPI_MakePolygon polygon;
        polygon.Add(A);
        polygon.Add(B);
        polygon.Add(C);
        polygon.Add(D);
        polygon.Close(); // 闭合线框

        TopoDS_Wire wire = polygon.Wire();
        gp_Pln plane(gp::YOZ());
        BRepBuilderAPI_MakeFace faceMaker(plane, wire);
        TopoDS_Face face = faceMaker.Face();
        TopoDS_Shape myShape = face;
        myAISContext()->Display(new AIS_Shape(myShape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateQuadYOZ

#if 0 // UNUSED_LEGACY: CreateQuadZOX
    void CreateQuadZOX(double X1, double Y1, double Z1, double X2, double Y2, double Z2, double X3, double Y3, double Z3, double X4, double Y4, double Z4)
    {
        gp_Pnt A(X1, Y1, Z1); // 左下
        gp_Pnt B(X2, Y2, Z2); // 左上
        gp_Pnt C(X3, Y3, Z3);  // 右上
        gp_Pnt D(X4, Y4, Z4);  // 右下

        BRepBuilderAPI_MakePolygon polygon;
        polygon.Add(A);
        polygon.Add(B);
        polygon.Add(C);
        polygon.Add(D);
        polygon.Close(); // 闭合线框

        TopoDS_Wire wire = polygon.Wire();
        gp_Pln plane(gp::ZOX());
        BRepBuilderAPI_MakeFace faceMaker(plane, wire);
        TopoDS_Face face = faceMaker.Face();
        TopoDS_Shape myShape = face;
        myAISContext()->Display(new AIS_Shape(myShape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateQuadZOX
// OLD_REGION: #pragma endregion
// OLD_REGION: #pragma region 创建T形函数，杨宇昂添加，2025.4
#if 0 // UNUSED_LEGACY: CreateXOZTShape
    void CreateXOZTShape(double X1, double Y1, double Z1, double X2, double Y2, double Z2, double X3, double Y3, double Z3, double X4, double Y4, double Z4, double X5, double Y5, double Z5, double X6, double Y6, double Z6, double X7, double Y7, double Z7, double X8, double Y8, double Z8)
    {
        gp_Pnt A(X1, Y1, Z1); // 左下
        gp_Pnt B(X2, Y2, Z2); // 左上
        gp_Pnt C(X3, Y3, Z3);  // 右上
        gp_Pnt D(X4, Y4, Z4);  // 右下
        gp_Pnt E(X5, Y5, Z5); // 左下
        gp_Pnt F(X6, Y6, Z6); // 左上
        gp_Pnt G(X7, Y7, Z7);  // 右上
        gp_Pnt H(X8, Y8, Z8);  // 右下
        // 创建多边形线框
        BRepBuilderAPI_MakePolygon polygon;
        polygon.Add(A);
        polygon.Add(B);
        polygon.Add(C);
        polygon.Add(D);
        polygon.Add(E);
        polygon.Add(F);
        polygon.Add(G);
        polygon.Add(H);
        polygon.Close(); // 闭合线框

        TopoDS_Wire wire = polygon.Wire();
        gp_Pln plane(gp::ZOX());
        BRepBuilderAPI_MakeFace faceMaker(plane, wire);
        TopoDS_Face face = faceMaker.Face();
        TopoDS_Shape myShape = face;
        myAISContext()->Display(new AIS_Shape(myShape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateXOZTShape

#if 0 // UNUSED_LEGACY: CreateXOYTShape
    void CreateXOYTShape(double X1, double Y1, double Z1, double X2, double Y2, double Z2, double X3, double Y3, double Z3, double X4, double Y4, double Z4, double X5, double Y5, double Z5, double X6, double Y6, double Z6, double X7, double Y7, double Z7, double X8, double Y8, double Z8)
    {
        gp_Pnt A(X1, Y1, Z1); // 左下
        gp_Pnt B(X2, Y2, Z2); // 左上
        gp_Pnt C(X3, Y3, Z3);  // 右上
        gp_Pnt D(X4, Y4, Z4);  // 右下
        gp_Pnt E(X5, Y5, Z5); // 左下
        gp_Pnt F(X6, Y6, Z6); // 左上
        gp_Pnt G(X7, Y7, Z7);  // 右上
        gp_Pnt H(X8, Y8, Z8);  // 右下
        // 创建多边形线框
        BRepBuilderAPI_MakePolygon polygon;
        polygon.Add(A);
        polygon.Add(B);
        polygon.Add(C);
        polygon.Add(D);
        polygon.Add(E);
        polygon.Add(F);
        polygon.Add(G);
        polygon.Add(H);
        polygon.Close(); // 闭合线框

        TopoDS_Wire wire = polygon.Wire();
        gp_Pln plane(gp::XOY());
        BRepBuilderAPI_MakeFace faceMaker(plane, wire);
        TopoDS_Face face = faceMaker.Face();
        TopoDS_Shape myShape = face;
        myAISContext()->Display(new AIS_Shape(myShape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateXOYTShape
    //创建圆弧函数，对接DXF解析与交互绘图， 国延林，2026.06 
    void CreateArcByCenter(int id, double cx, double cy, double cz, double radius, double startAngle, double endAngle)
    {
        gp_Pnt center(cx, cy, cz);
        gp_Dir normal(0.0, 0.0, 1.0);
        gp_Ax2 axis(center, normal);
        gp_Circ aCircle(axis, radius);

        Standard_Real alpha1 = startAngle * 3.14159265358979323846 / 180.0;
        Standard_Real alpha2 = endAngle * 3.14159265358979323846 / 180.0;

        GC_MakeArcOfCircle makeArc(aCircle, alpha1, alpha2, Standard_True);
        if (makeArc.IsDone())
        {
            TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge(makeArc.Value());
            Handle(AIS_Shape) aisShape = new AIS_Shape(anEdge);

            // 【修改点 2】：存入档案室
            GlobalShapeMap[id] = aisShape;

            myAISContext()->Display(aisShape, Standard_False);
        }
    }
    //根据【起点、中点、终点】三点创建几何圆弧（用于手动交互绘图）
#if 0 // UNUSED_LEGACY: CreateArcBy3Points
    void CreateArcBy3Points(double x1, double y1, double z1,
        double x2, double y2, double z2,
        double x3, double y3, double z3)
    {
        gp_Pnt p1(x1, y1, z1); // 起点
        gp_Pnt p2(x2, y2, z2); // 弧上任意中点
        gp_Pnt p3(x3, y3, z3); // 终点

        // 使用三点直接构造圆弧
        GC_MakeArcOfCircle makeArc(p1, p2, p3);

        if (makeArc.IsDone())
        {
            TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge(makeArc.Value());
            myAISContext()->Display(new AIS_Shape(anEdge), Standard_False);
            myAISContext()->UpdateCurrentViewer();
        }
    }
#endif // UNUSED_LEGACY: CreateArcBy3Points
#if 0 // UNUSED_LEGACY: CreateYOZTShape
    void CreateYOZTShape(double X1, double Y1, double Z1, double X2, double Y2, double Z2, double X3, double Y3, double Z3, double X4, double Y4, double Z4, double X5, double Y5, double Z5, double X6, double Y6, double Z6, double X7, double Y7, double Z7, double X8, double Y8, double Z8)
    {
        gp_Pnt A(X1, Y1, Z1); // 左下
        gp_Pnt B(X2, Y2, Z2); // 左上
        gp_Pnt C(X3, Y3, Z3);  // 右上
        gp_Pnt D(X4, Y4, Z4);  // 右下
        gp_Pnt E(X5, Y5, Z5); // 左下
        gp_Pnt F(X6, Y6, Z6); // 左上
        gp_Pnt G(X7, Y7, Z7);  // 右上
        gp_Pnt H(X8, Y8, Z8);  // 右下
        // 创建多边形线框
        BRepBuilderAPI_MakePolygon polygon;
        polygon.Add(A);
        polygon.Add(B);
        polygon.Add(C);
        polygon.Add(D);
        polygon.Add(E);
        polygon.Add(F);
        polygon.Add(G);
        polygon.Add(H);
        polygon.Close(); // 闭合线框

        TopoDS_Wire wire = polygon.Wire();
        gp_Pln plane(gp::YOZ());
        BRepBuilderAPI_MakeFace faceMaker(plane, wire);
        TopoDS_Face face = faceMaker.Face();
        TopoDS_Shape myShape = face;
        myAISContext()->Display(new AIS_Shape(myShape), Standard_False);
        myAISContext()->UpdateCurrentViewer();
    }
#endif // UNUSED_LEGACY: CreateYOZTShape
#pragma endregion // 03_Basic_Geometry_And_Disabled_Legacy_Drawing
    /// <summary>
    /// 根据 ID 隐藏或显示指定的图元
    /// </summary>
#pragma region 04_Object_Visibility_And_Lifecycle
    void SetShapeVisibility(int id, bool isVisible)
    {
        if (myAISContext().IsNull()) return;

        // 去档案室里找这个 ID
        auto it = GlobalShapeMap.find(id);
        if (it != GlobalShapeMap.end())
        {
            if (isVisible) {
                myAISContext()->Display(it->second, Standard_False);
            }
            else {
                myAISContext()->Erase(it->second, Standard_False);
            }
        }
    }

    /// <summary>
    /// 覆盖你之前的 ClearAll，确保清空画面时，档案室也被清空，防止内存爆满 国延林 2026.6
    /// </summary>
    void ClearAll(void)
    {
        if (myAISContext().IsNull()) return;
        for (auto it = GlobalShapeMap.begin(); it != GlobalShapeMap.end(); ++it)
        {
            myAISContext()->Remove(it->second, Standard_False);
        }
        GlobalShapeMap.clear();
        myAISContext()->UpdateCurrentViewer();
    }

    /// <summary>
    /// 清除当前 3D 视口中所有的选中高亮状态 (供 C# 端调用)
    /// </summary>
    void ClearSelected(void)
    {
        if (!myAISContext().IsNull())
        {
            myAISContext()->ClearSelected(Standard_False); // 清空底层选择集
            myAISContext()->UpdateCurrentViewer();         // 强制刷新画面，让高亮立刻消失
        }
    }
#pragma endregion // 04_Object_Visibility_And_Lifecycle



    //线条自动连接（帘线专用）
#pragma region 05_Line_Combine_Split_And_Delete
    array<int>^ AutoCombineLines(int startId, int newId)
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
    bool CombineLines(int id1, int id2, int newId)
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
    /// 智能链选合并：点击一根线，自动蔓延寻找所有相连线段，并熔铸为单一曲线
    /// </summary>
    /// <returns>返回被自动消耗掉的所有散线 ID 数组。如果失败返回 nullptr</returns>
   /* array<int>^ AutoTraceAndMergeBoundaryLine(int startId, int newId)
    {
        return AutoTraceAndMergeBoundaryLine(startId, newId, 0.05, 60.0);
    }

    array<int>^ AutoTraceAndMergeBoundaryLine(
        int startId,
        int newId,
        double snapTol,
        double maxTurnAngleDeg)
    {
        if (myAISContext().IsNull())
            return nullptr;

        if (GlobalShapeMap.find(startId) == GlobalShapeMap.end())
            return nullptr;

        if (snapTol <= Precision::Confusion())
            snapTol = 1.0e-2;

        if (maxTurnAngleDeg <= 0.0 || maxTurnAngleDeg >= 180.0)
            maxTurnAngleDeg = 60.0;

        Handle(AIS_InteractiveObject) startObj = GlobalShapeMap[startId];

        if (startObj.IsNull())
            return nullptr;

        if (!myAISContext()->IsDisplayed(startObj))
            return nullptr;

        TopoDS_Edge seedEdge;

        if (!GetFirstEdgeFromAISObject(startObj, seedEdge))
            return nullptr;

        gp_Pnt seedP0;
        gp_Pnt seedP1;

        if (!GetEdgeEndPoints(seedEdge, seedP0, seedP1))
            return nullptr;

        gp_Vec seedDir(seedP0, seedP1);

        if (seedDir.SquareMagnitude() < 1.0e-18)
            return nullptr;

        seedDir.Normalize();

        Standard_Real seedLen = seedP0.Distance(seedP1);
        if (seedLen < snapTol * 5.0)
            seedLen = snapTol * 20.0;

        // 防止 newId 和即将被追踪的旧线 id 冲突。
        if (newId == startId)
            return nullptr;

        // ============================================================
        // 1. 从 seedEdge 两端分别进行有方向追踪
        // ============================================================

        std::set<int> usedIds;
        usedIds.insert(startId);

        std::vector<BoundaryTraceItem> leftItems;
        std::vector<BoundaryTraceItem> rightItems;

        gp_Vec leftStartDir = seedDir.Reversed();
        gp_Vec rightStartDir = seedDir;

        TraceOneSideByDirection(
            myAISContext(),
            seedP0,
            leftStartDir,
            seedLen,
            snapTol,
            maxTurnAngleDeg,
            usedIds,
            leftItems);

        TraceOneSideByDirection(
            myAISContext(),
            seedP1,
            rightStartDir,
            seedLen,
            snapTol,
            maxTurnAngleDeg,
            usedIds,
            rightItems);

        // 如果左右都没有追踪到新线，说明 startId 本身是孤立线。
        if (leftItems.empty() && rightItems.empty())
            return nullptr;

        // ============================================================
        // 2. 组织成一条从左端到右端的有序链
        // ============================================================

        std::vector<BoundaryTraceItem> orderedItems;

        // leftItems 的方向是：seedP0 -> 外侧。
        // 真正拼线时要从最外侧 -> seedP0，所以需要倒序并反向。
        for (int i = static_cast<int>(leftItems.size()) - 1; i >= 0; --i)
        {
            BoundaryTraceItem reversedItem;
            ReverseTraceItem(leftItems[i], reversedItem);
            orderedItems.push_back(reversedItem);
        }

        BoundaryTraceItem seedItem;
        seedItem.id = startId;
        seedItem.edge = seedEdge;
        seedItem.fromP = seedP0;
        seedItem.toP = seedP1;

        orderedItems.push_back(seedItem);

        // rightItems 本来就是 seedP1 -> 外侧，直接接上。
        for (std::size_t i = 0; i < rightItems.size(); ++i)
        {
            orderedItems.push_back(rightItems[i]);
        }

        if (orderedItems.size() < 2)
            return nullptr;

        // ============================================================
        // 3. 把有序 Edge 链融合成单根 B-Spline
        // ============================================================

        Handle(Geom_BSplineCurve) finalBSpline =
            BuildSingleBSplineFromOrderedTraceItems(orderedItems, snapTol);

        if (finalBSpline.IsNull())
            return nullptr;

        TopoDS_Edge finalSingleEdge =
            BRepBuilderAPI_MakeEdge(finalBSpline);

        if (finalSingleEdge.IsNull())
            return nullptr;

        // ============================================================
        // 4. 创建新的 AIS 对象
        // ============================================================

        Handle(AIS_Shape) newAis = new AIS_Shape(finalSingleEdge);
        newAis->SetColor(Quantity_NOC_RED);
        //newAis->SetWidth(2.0);

        // 如果 newId 已经存在，并且不是本次被消耗的旧线，则先移除旧对象。
        if (GlobalShapeMap.find(newId) != GlobalShapeMap.end())
        {
            if (!GlobalShapeMap[newId].IsNull())
                myAISContext()->Remove(GlobalShapeMap[newId], Standard_False);

            GlobalShapeMap.erase(newId);
        }

        // ============================================================
        // 5. 旧线处理
        //
        // 这里默认“隐藏旧线，不销毁旧线”，这样更安全，方便你以后做撤销。
        // 如果你想彻底删除旧线，看下面注释位置。
        // ============================================================

        std::vector<int> consumedIds;
        std::set<int> consumedSet;

        for (std::size_t i = 0; i < orderedItems.size(); ++i)
        {
            int oldId = orderedItems[i].id;

            if (consumedSet.find(oldId) != consumedSet.end())
                continue;

            consumedSet.insert(oldId);
            consumedIds.push_back(oldId);
        }

        array<int>^ consumedArray =
            gcnew array<int>(static_cast<int>(consumedIds.size()));

        for (std::size_t i = 0; i < consumedIds.size(); ++i)
        {
            int oldId = consumedIds[i];
            consumedArray[static_cast<int>(i)] = oldId;

            if (GlobalShapeMap.find(oldId) == GlobalShapeMap.end())
                continue;

            if (GlobalShapeMap[oldId].IsNull())
                continue;

            // 推荐：隐藏旧线，保留 GlobalShapeMap 记录，方便撤销。
            myAISContext()->Erase(GlobalShapeMap[oldId], Standard_False);

            // 如果你想像旧代码一样彻底删除旧线，把上面 Erase 注释掉，
            // 然后打开下面两行：
            //
            // myAISContext()->Remove(GlobalShapeMap[oldId], Standard_False);
            // GlobalShapeMap.erase(oldId);
        }

        // ============================================================
        // 6. 登记并显示新合并线
        // ============================================================

        GlobalShapeMap[newId] = newAis;

        myAISContext()->Display(newAis, Standard_False);
        myAISContext()->ClearSelected(Standard_False);
        myAISContext()->UpdateCurrentViewer();

        return consumedArray;
    }*/
    /// <summary>
    /// 线打断
    /// </summary>
    /// <param name="targetId">要被打断的线 (Argument)</param>
    /// <param name="toolId">作为切刀的相交线 (Tool)</param>
    /// <param name="startingNewId">由 C# 传入的可用起始 ID</param>
    /// <returns>返回打断后生成的所有新散线 ID 数组。如果没相交/失败，返回 nullptr</returns>
    array<int>^ SplitLine(
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
    bool DeleteLine(int id)
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
#pragma endregion // 05_Line_Combine_Split_And_Delete




    /// <summary>
    /// 核心制面引擎：将传入的多个线段 ID 缝合并生成实心面
    /// </summary>
#pragma region 06_Face_Polyline_Cord_And_Selection
    bool CreateFaceFromLines(
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

    /// <summary>
    ///将打散后的多段线重新合成为一个整体
    /// </summary>
    void CreatePolyline(int id, array<double>^ coords, bool isClosed)
    {
        if (myAISContext().IsNull() || coords->Length < 6) return; // 至少需要2个点

        // 1. 先用 MakePolygon 把离散点临时连成拓扑线框 (Wire)
        BRepBuilderAPI_MakePolygon polygonMaker;
        for (int i = 0; i < coords->Length; i += 3)
        {
            polygonMaker.Add(gp_Pnt(coords[i], coords[i + 1], coords[i + 2]));
        }

        if (isClosed) {
            polygonMaker.Close();
        }

        if (!polygonMaker.IsDone()) return;
        TopoDS_Wire rawWire = polygonMaker.Wire();

        BRepTools_WireExplorer wireExp(rawWire);
        if (!wireExp.More()) return;

        // a. 取出第一小段作为“熔炉”的基底
        TopoDS_Edge currentEdge = wireExp.Current();
        Standard_Real f, l;
        Handle(Geom_Curve) currentCurve = BRep_Tool::Curve(currentEdge, f, l);
        Handle(Geom_TrimmedCurve) trimmedCurve = new Geom_TrimmedCurve(currentCurve, f, l);

        if (currentEdge.Orientation() == TopAbs_REVERSED) {
            trimmedCurve->Reverse();
        }

        GeomConvert_CompCurveToBSplineCurve concatenator(trimmedCurve);
        wireExp.Next();

        // b. 遍历剩下的线段，在数学底层全部无缝“焊”成一条 B-Spline
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

        // c. 获取最终的纯数学形态：一条连续无内部顶点的单一曲线
        Handle(Geom_BSplineCurve) singleBSpline = concatenator.BSplineCurve();

        // d. 将这条数学曲线重新包装成【唯一的一个 Edge】
        TopoDS_Edge finalSingleEdge = BRepBuilderAPI_MakeEdge(singleBSpline);
        // ==============================================================

        // 3. 包装显示并交接户籍 (此时放入档案室的，是纯粹的一根 Edge！)
        Handle(AIS_Shape) aisShape = new AIS_Shape(finalSingleEdge);

        GlobalShapeMap[id] = aisShape;
        myAISContext()->Display(aisShape, Standard_False);
    }

    /// <summary>
    /// 基于等弧长参数化离散的双边界中位线拟合算子
    /// </summary>
    /// <param name="id1">边界曲线 A 的全局实体标识符</param>
    /// <param name="id2">边界曲线 B 的全局实体标识符</param>
    /// <param name="newId">分配予生成的帘线实体的全新标识符</param>
    /// <returns>几何拟合与拓扑注册是否成功</returns>
    bool CreateCordFromTwoCurves(int id1, int id2, int newId)
    {
        if (myAISContext().IsNull())
            return false;

        if (GlobalShapeMap.find(id1) == GlobalShapeMap.end() ||
            GlobalShapeMap.find(id2) == GlobalShapeMap.end())
            return false;

        Handle(AIS_Shape) ais1 = Handle(AIS_Shape)::DownCast(GlobalShapeMap[id1]);
        Handle(AIS_Shape) ais2 = Handle(AIS_Shape)::DownCast(GlobalShapeMap[id2]);

        if (ais1.IsNull() || ais2.IsNull())
            return false;

        TopoDS_Shape shape1 = ais1->Shape();
        TopoDS_Shape shape2 = ais2->Shape();

        TopExp_Explorer exp1(shape1, TopAbs_EDGE);
        TopExp_Explorer exp2(shape2, TopAbs_EDGE);

        if (!exp1.More() || !exp2.More())
            return false;

        TopoDS_Edge edge1 = TopoDS::Edge(exp1.Current());
        TopoDS_Edge edge2 = TopoDS::Edge(exp2.Current());

        Standard_Real f1 = 0.0, l1 = 0.0;
        Standard_Real f2 = 0.0, l2 = 0.0;

        TopLoc_Location loc1;
        TopLoc_Location loc2;

        Handle(Geom_Curve) curve1 = BRep_Tool::Curve(edge1, loc1, f1, l1);
        Handle(Geom_Curve) curve2 = BRep_Tool::Curve(edge2, loc2, f2, l2);

        if (curve1.IsNull() || curve2.IsNull())
            return false;

        if (!loc1.IsIdentity())
        {
            Handle(Geom_Geometry) g1 = curve1->Transformed(loc1.Transformation());
            curve1 = Handle(Geom_Curve)::DownCast(g1);

            if (curve1.IsNull())
                return false;
        }

        if (!loc2.IsIdentity())
        {
            Handle(Geom_Geometry) g2 = curve2->Transformed(loc2.Transformation());
            curve2 = Handle(Geom_Curve)::DownCast(g2);

            if (curve2.IsNull())
                return false;
        }

        if (l1 <= f1 || l2 <= f2)
            return false;

        // ============================================================
        // 1. 方向对齐，但不直接 Reverse 曲线，避免参数域错乱
        // ============================================================

        gp_Pnt start1 = curve1->Value(f1);
        gp_Pnt end1 = curve1->Value(l1);
        gp_Pnt start2 = curve2->Value(f2);
        gp_Pnt end2 = curve2->Value(l2);

        Standard_Real sameCost =
            start1.SquareDistance(start2) +
            end1.SquareDistance(end2);

        Standard_Real reverseCost =
            start1.SquareDistance(end2) +
            end1.SquareDistance(start2);

        Standard_Boolean curve2Reversed = reverseCost < sameCost;

        GeomAdaptor_Curve gac1(curve1, f1, l1);
        GeomAdaptor_Curve gac2(curve2, f2, l2);

        Standard_Real len1 = GCPnts_AbscissaPoint::Length(gac1, f1, l1);
        Standard_Real len2 = GCPnts_AbscissaPoint::Length(gac2, f2, l2);

        if (len1 <= Precision::Confusion() ||
            len2 <= Precision::Confusion())
            return false;

        Handle(Geom_Curve) driverCurve;
        Handle(Geom_Curve) targetCurve;

        Standard_Real df = 0.0, dl = 0.0;
        Standard_Real tf = 0.0, tl = 0.0;

        Standard_Boolean driverReversed = Standard_False;
        Standard_Boolean targetReversed = Standard_False;

        if (len1 <= len2)
        {
            driverCurve = curve1;
            df = f1;
            dl = l1;
            driverReversed = Standard_False;

            targetCurve = curve2;
            tf = f2;
            tl = l2;
            targetReversed = curve2Reversed;
        }
        else
        {
            driverCurve = curve2;
            df = f2;
            dl = l2;
            driverReversed = curve2Reversed;

            targetCurve = curve1;
            tf = f1;
            tl = l1;
            targetReversed = Standard_False;
        }

        // ============================================================
        // 2. 离散采样
        // ============================================================

        const Standard_Integer N = 220;
        const Standard_Integer M = 2200;

        std::vector<gp_Pnt> driverPnts(N + 1);
        std::vector<gp_Vec> driverTans(N + 1);
        std::vector<Standard_Real> driverRatio(N + 1);

        std::vector<gp_Pnt> targetPnts(M + 1);
        std::vector<gp_Vec> targetTans(M + 1);
        std::vector<Standard_Real> targetRatio(M + 1);

        {
            GeomAdaptor_Curve gac(driverCurve, df, dl);
            GCPnts_UniformAbscissa uni(gac, N, df, dl);

            if (!uni.IsDone() || uni.NbPoints() < 2)
                return false;

            for (Standard_Integer i = 1; i <= N; ++i)
            {
                Standard_Real ratio =
                    static_cast<Standard_Real>(i - 1) /
                    static_cast<Standard_Real>(N - 1);

                Standard_Integer idx = driverReversed ? (N - i + 1) : i;
                Standard_Real u = uni.Parameter(idx);

                gp_Pnt p;
                gp_Vec t;

                driverCurve->D1(u, p, t);

                if (driverReversed)
                    t.Reverse();

                if (t.SquareMagnitude() < 1.0e-18)
                    return false;

                t.Normalize();

                driverPnts[i] = p;
                driverTans[i] = t;
                driverRatio[i] = ratio;
            }
        }

        {
            GeomAdaptor_Curve gac(targetCurve, tf, tl);
            GCPnts_UniformAbscissa uni(gac, M, tf, tl);

            if (!uni.IsDone() || uni.NbPoints() < 2)
                return false;

            for (Standard_Integer j = 1; j <= M; ++j)
            {
                Standard_Real ratio =
                    static_cast<Standard_Real>(j - 1) /
                    static_cast<Standard_Real>(M - 1);

                Standard_Integer idx = targetReversed ? (M - j + 1) : j;
                Standard_Real u = uni.Parameter(idx);

                gp_Pnt p;
                gp_Vec t;

                targetCurve->D1(u, p, t);

                if (targetReversed)
                    t.Reverse();

                if (t.SquareMagnitude() < 1.0e-18)
                    return false;

                t.Normalize();

                targetPnts[j] = p;
                targetTans[j] = t;
                targetRatio[j] = ratio;
            }
        }

        // ============================================================
        // 3. 用中间区域估算正常帘线宽度
        // ============================================================

        std::vector<Standard_Real> gapSamples;

        for (Standard_Integer i = 1; i <= N; ++i)
        {
            Standard_Real r = driverRatio[i];

            if (r < 0.25 || r > 0.75)
                continue;

            Standard_Integer j =
                static_cast<Standard_Integer>(
                    std::floor(r * static_cast<Standard_Real>(M - 1) + 1.5));

            if (j < 1) j = 1;
            if (j > M) j = M;

            Standard_Real d = driverPnts[i].Distance(targetPnts[j]);

            if (d > Precision::Confusion())
                gapSamples.push_back(d);
        }

        if (gapSamples.empty())
            return false;

        std::sort(gapSamples.begin(), gapSamples.end());

        Standard_Real typicalGap = 0.0;
        std::size_t gapCount = gapSamples.size();

        if (gapCount % 2 == 1)
            typicalGap = gapSamples[gapCount / 2];
        else
            typicalGap = 0.5 * (gapSamples[gapCount / 2 - 1] + gapSamples[gapCount / 2]);

        if (typicalGap <= Precision::Confusion())
            return false;

        // ============================================================
        // 4. 匹配 target 点
        //    注意：这里不删除两端，只标记两端是否异常
        // ============================================================

        std::vector<gp_Pnt> rawMidPoints(N + 1);
        std::vector<Standard_Real> chordLens(N + 1, 0.0);
        std::vector<Standard_Real> dotDrivers(N + 1, 1.0);
        std::vector<Standard_Real> dotTargets(N + 1, 1.0);
        std::vector<Standard_Real> tangentDots(N + 1, 0.0);
        std::vector<Standard_Integer> matchedJ(N + 1, 1);
        std::vector<Standard_Boolean> good(N + 1, Standard_False);

        const Standard_Real maxAllowedGap = typicalGap * 3.0;
        const Standard_Real minAllowedGap = typicalGap * 0.05;

        const Standard_Integer forwardWindow =
            std::max<Standard_Integer>(120, M / 5);

        const Standard_Integer ratioWindow =
            std::max<Standard_Integer>(80, M / 12);

        const Standard_Integer backTrack = 8;

        Standard_Integer currentJ = 1;

        for (Standard_Integer i = 1; i <= N; ++i)
        {
            Standard_Real r = driverRatio[i];

            Standard_Integer centerJ =
                static_cast<Standard_Integer>(
                    std::floor(r * static_cast<Standard_Real>(M - 1) + 1.5));

            if (centerJ < 1) centerJ = 1;
            if (centerJ > M) centerJ = M;

            Standard_Integer lowerJ = currentJ - backTrack;
            if (lowerJ < 1) lowerJ = 1;

            Standard_Integer upperJ = currentJ + forwardWindow;
            if (upperJ < centerJ + ratioWindow)
                upperJ = centerJ + ratioWindow;

            if (upperJ > M)
                upperJ = M;

            Standard_Real bestScore = 1.0e100;
            Standard_Integer bestJ = currentJ;

            for (Standard_Integer j = lowerJ; j <= upperJ; ++j)
            {
                gp_Vec chord(driverPnts[i], targetPnts[j]);
                Standard_Real chordLen = chord.Magnitude();

                if (chordLen <= Precision::Confusion())
                    continue;

                // 不硬删除，但过大的厚度要强惩罚，避免跨到端部封边
                gp_Vec chordDir = chord;
                chordDir.Normalize();

                Standard_Real dotD = std::fabs(driverTans[i].Dot(chordDir));
                Standard_Real dotT = std::fabs(targetTans[j].Dot(chordDir));
                Standard_Real tanDot = std::fabs(driverTans[i].Dot(targetTans[j]));

                Standard_Real gapErr =
                    std::fabs(chordLen - typicalGap) / typicalGap;

                Standard_Real ratioErr =
                    std::fabs(static_cast<Standard_Real>(j - centerJ)) /
                    static_cast<Standard_Real>(std::max<Standard_Integer>(1, ratioWindow));

                Standard_Real distScore =
                    driverPnts[i].SquareDistance(targetPnts[j]) /
                    (typicalGap * typicalGap);

                Standard_Real overGapPenalty = 0.0;
                if (chordLen > maxAllowedGap)
                {
                    Standard_Real x = (chordLen - maxAllowedGap) / typicalGap;
                    overGapPenalty = 100.0 * x * x;
                }

                Standard_Real underGapPenalty = 0.0;
                if (chordLen < minAllowedGap)
                {
                    Standard_Real x = (minAllowedGap - chordLen) / typicalGap;
                    underGapPenalty = 100.0 * x * x;
                }

                Standard_Real score =
                    1.00 * distScore +
                    5.00 * gapErr * gapErr +
                    8.00 * (dotD * dotD + dotT * dotT) +
                    2.00 * (1.0 - tanDot) * (1.0 - tanDot) +
                    0.30 * ratioErr * ratioErr +
                    overGapPenalty +
                    underGapPenalty;

                if (score < bestScore)
                {
                    bestScore = score;
                    bestJ = j;
                }
            }

            if (bestJ < currentJ)
                bestJ = currentJ;

            currentJ = bestJ;

            gp_Pnt pD = driverPnts[i];
            gp_Pnt pT = targetPnts[bestJ];

            gp_Vec chord(pD, pT);
            Standard_Real chordLen = chord.Magnitude();

            Standard_Real dotD = 1.0;
            Standard_Real dotT = 1.0;
            Standard_Real tanDot = 0.0;

            if (chordLen > Precision::Confusion())
            {
                gp_Vec chordDir = chord;
                chordDir.Normalize();

                dotD = std::fabs(driverTans[i].Dot(chordDir));
                dotT = std::fabs(targetTans[bestJ].Dot(chordDir));
                tanDot = std::fabs(driverTans[i].Dot(targetTans[bestJ]));
            }

            rawMidPoints[i].SetCoord(
                0.5 * (pD.X() + pT.X()),
                0.5 * (pD.Y() + pT.Y()),
                0.5 * (pD.Z() + pT.Z()));

            chordLens[i] = chordLen;
            dotDrivers[i] = dotD;
            dotTargets[i] = dotT;
            tangentDots[i] = tanDot;
            matchedJ[i] = bestJ;

            if (chordLen >= typicalGap * 0.20 &&
                chordLen <= typicalGap * 2.20 &&
                dotD <= 0.60 &&
                dotT <= 0.70 &&
                tanDot >= 0.20)
            {
                good[i] = Standard_True;
            }
        }

        // ============================================================
        // 5. 找中间稳定段
        //    两端异常不删除，而是后面修复
        // ============================================================

        const Standard_Integer stableRun = 8;

        Standard_Integer firstStable = 1;
        Standard_Integer lastStable = N;

        for (Standard_Integer i = 1; i <= N - stableRun + 1; ++i)
        {
            Standard_Boolean ok = Standard_True;

            for (Standard_Integer k = 0; k < stableRun; ++k)
            {
                if (!good[i + k])
                {
                    ok = Standard_False;
                    break;
                }
            }

            if (ok)
            {
                firstStable = i;
                break;
            }
        }

        for (Standard_Integer i = N - stableRun + 1; i >= 1; --i)
        {
            Standard_Boolean ok = Standard_True;

            for (Standard_Integer k = 0; k < stableRun; ++k)
            {
                if (!good[i + k])
                {
                    ok = Standard_False;
                    break;
                }
            }

            if (ok)
            {
                lastStable = i + stableRun - 1;
                break;
            }
        }

        if (firstStable >= lastStable)
        {
            firstStable = 1;
            lastStable = N;
        }

        // 起点和终点使用真实两侧端点中点，而不是直接丢弃
        gp_Pnt startMid(
            0.5 * (driverPnts[1].X() + targetPnts[1].X()),
            0.5 * (driverPnts[1].Y() + targetPnts[1].Y()),
            0.5 * (driverPnts[1].Z() + targetPnts[1].Z()));

        gp_Pnt endMid(
            0.5 * (driverPnts[N].X() + targetPnts[M].X()),
            0.5 * (driverPnts[N].Y() + targetPnts[M].Y()),
            0.5 * (driverPnts[N].Z() + targetPnts[M].Z()));

        std::vector<gp_Pnt> fixedMidPoints(N + 1);

        for (Standard_Integer i = 1; i <= N; ++i)
            fixedMidPoints[i] = rawMidPoints[i];

        // ============================================================
        // 6. 修复起始端：不是删除，而是从端点中点平滑过渡到稳定段
        // ============================================================

        if (firstStable > 2 && firstStable < N)
        {
            gp_Pnt P0 = startMid;
            gp_Pnt P1 = rawMidPoints[firstStable];

            gp_Vec tan(P0, P1);

            if (firstStable + 1 <= N)
            {
                gp_Vec t2(rawMidPoints[firstStable], rawMidPoints[firstStable + 1]);
                if (t2.SquareMagnitude() > 1.0e-18)
                    tan = t2;
            }

            if (tan.SquareMagnitude() > 1.0e-18)
                tan.Normalize();

            Standard_Real L = P0.Distance(P1);
            Standard_Real scale = L * 0.55;

            for (Standard_Integer i = 1; i < firstStable; ++i)
            {
                Standard_Real u =
                    static_cast<Standard_Real>(i - 1) /
                    static_cast<Standard_Real>(firstStable - 1);

                Standard_Real u2 = u * u;
                Standard_Real u3 = u2 * u;

                Standard_Real h00 = 2.0 * u3 - 3.0 * u2 + 1.0;
                Standard_Real h10 = u3 - 2.0 * u2 + u;
                Standard_Real h01 = -2.0 * u3 + 3.0 * u2;
                Standard_Real h11 = u3 - u2;

                Standard_Real x =
                    h00 * P0.X() +
                    h10 * scale * tan.X() +
                    h01 * P1.X() +
                    h11 * scale * tan.X();

                Standard_Real y =
                    h00 * P0.Y() +
                    h10 * scale * tan.Y() +
                    h01 * P1.Y() +
                    h11 * scale * tan.Y();

                Standard_Real z =
                    h00 * P0.Z() +
                    h10 * scale * tan.Z() +
                    h01 * P1.Z() +
                    h11 * scale * tan.Z();

                fixedMidPoints[i].SetCoord(x, y, z);
            }
        }

        // ============================================================
        // 7. 修复结束端：同样平滑补到真实端点中点
        // ============================================================

        if (lastStable < N - 1 && lastStable > 1)
        {
            gp_Pnt P0 = rawMidPoints[lastStable];
            gp_Pnt P1 = endMid;

            gp_Vec tan(P0, P1);

            if (lastStable - 1 >= 1)
            {
                gp_Vec t2(rawMidPoints[lastStable - 1], rawMidPoints[lastStable]);
                if (t2.SquareMagnitude() > 1.0e-18)
                    tan = t2;
            }

            if (tan.SquareMagnitude() > 1.0e-18)
                tan.Normalize();

            Standard_Real L = P0.Distance(P1);
            Standard_Real scale = L * 0.55;

            Standard_Integer count = N - lastStable;

            for (Standard_Integer i = lastStable + 1; i <= N; ++i)
            {
                Standard_Real u =
                    static_cast<Standard_Real>(i - lastStable) /
                    static_cast<Standard_Real>(count);

                Standard_Real u2 = u * u;
                Standard_Real u3 = u2 * u;

                Standard_Real h00 = 2.0 * u3 - 3.0 * u2 + 1.0;
                Standard_Real h10 = u3 - 2.0 * u2 + u;
                Standard_Real h01 = -2.0 * u3 + 3.0 * u2;
                Standard_Real h11 = u3 - u2;

                Standard_Real x =
                    h00 * P0.X() +
                    h10 * scale * tan.X() +
                    h01 * P1.X() +
                    h11 * scale * tan.X();

                Standard_Real y =
                    h00 * P0.Y() +
                    h10 * scale * tan.Y() +
                    h01 * P1.Y() +
                    h11 * scale * tan.Y();

                Standard_Real z =
                    h00 * P0.Z() +
                    h10 * scale * tan.Z() +
                    h01 * P1.Z() +
                    h11 * scale * tan.Z();

                fixedMidPoints[i].SetCoord(x, y, z);
            }
        }

        // ============================================================
        // 8. 清理重复点，但不裁剪两端
        // ============================================================

        std::vector<gp_Pnt> cleanPoints;
        cleanPoints.reserve(N);

        Standard_Real duplicateTol =
            std::max(Precision::Confusion() * 10.0, typicalGap * 1.0e-6);

        for (Standard_Integer i = 1; i <= N; ++i)
        {
            if (cleanPoints.empty())
            {
                cleanPoints.push_back(fixedMidPoints[i]);
            }
            else
            {
                if (cleanPoints.back().Distance(fixedMidPoints[i]) > duplicateTol)
                    cleanPoints.push_back(fixedMidPoints[i]);
            }
        }

        if (cleanPoints.size() < 2)
            return false;

        Handle(TColgp_HArray1OfPnt) finalMidPoints =
            new TColgp_HArray1OfPnt(
                1,
                static_cast<Standard_Integer>(cleanPoints.size()));

        for (Standard_Integer i = 0;
            i < static_cast<Standard_Integer>(cleanPoints.size());
            ++i)
        {
            finalMidPoints->SetValue(i + 1, cleanPoints[i]);
        }

        // ============================================================
        // 9. 拟合
        // ============================================================

        Standard_Real fitTol =
            std::max(Precision::Confusion(), typicalGap * 0.0005);

        GeomAPI_PointsToBSpline bsplineMaker(
            finalMidPoints->Array1(),
            3,
            3,
            GeomAbs_C1,
            fitTol);

        if (!bsplineMaker.IsDone())
            return false;

        TopoDS_Edge cordEdge = BRepBuilderAPI_MakeEdge(bsplineMaker.Curve());

        Handle(AIS_Shape) cordAis = new AIS_Shape(cordEdge);
        cordAis->SetColor(Quantity_NOC_GREEN);
        cordAis->SetWidth(2.0);

        if (GlobalShapeMap.find(newId) != GlobalShapeMap.end())
        {
            if (!GlobalShapeMap[newId].IsNull())
                myAISContext()->Remove(GlobalShapeMap[newId], Standard_False);

            GlobalShapeMap.erase(newId);
        }

        GlobalShapeMap[newId] = cordAis;

        myAISContext()->Display(cordAis, Standard_False);
        myAISContext()->ClearSelected(Standard_False);
        myAISContext()->UpdateCurrentViewer();

        return true;
    }











    /// <summary>
    /// 获取当前在 3D 视口中被选中的图形的 ID 列表 (供 C# 端调用)
    /// </summary>
    System::Collections::Generic::List<int>^ GetSelectedIds()
    {
        // 创建一个 C# 能认识的 List<int>
        System::Collections::Generic::List<int>^ idList = gcnew System::Collections::Generic::List<int>();

        if (myAISContext().IsNull())
        {
            return idList;
        }

        // 遍历当前 OCC 引擎中所有被选中的物体
        for (myAISContext()->InitSelected(); myAISContext()->MoreSelected(); myAISContext()->NextSelected())
        {
            Handle(AIS_InteractiveObject) selectedObj = myAISContext()->SelectedInteractive();

            // 在我们的 ID 档案室 (GlobalShapeMap) 中反查这个物体的 ID
            for (auto it = GlobalShapeMap.begin(); it != GlobalShapeMap.end(); ++it)
            {
                if (it->second == selectedObj)
                {
                    // 找到了对应的 ID，装进要返回的列表中
                    idList->Add(it->first);
                    break; // 找齐了当前物体的 ID，跳出内层查找循环
                }
            }
        }

        return idList; // 返回给 C#
    }
    /// <summary>
    /// 强行将指定 ID 的物体加入当前 OCC 的选中亮显列表中
    /// </summary>
    void AddToSelection(int id)
    {
        if (myAISContext().IsNull()) return;
        if (GlobalShapeMap.find(id) != GlobalShapeMap.end()) {
            // AddOrRemoveSelected 会反转选中状态，前提是先 ClearSelected
            myAISContext()->AddOrRemoveSelected(GlobalShapeMap[id], Standard_False);
        }
    }
#pragma endregion // 06_Face_Polyline_Cord_And_Selection






// OLD_REGION: #pragma endregion
private:
    // fields
    NCollection_Haft<Handle(V3d_Viewer)> myViewer;
    NCollection_Haft<Handle(V3d_View)> myView;
    NCollection_Haft<Handle(AIS_InteractiveContext)> myAISContext;
    NCollection_Haft<Handle(OpenGl_GraphicDriver)> myGraphicDriver;
    //NCollection_Haft<Handle(STEPCAFControl_Reader)> mySTEPCAFControl_Reader;
    //NCollection_Haft<Handle(TDocStd_Document)> myOCCDoc;
};
