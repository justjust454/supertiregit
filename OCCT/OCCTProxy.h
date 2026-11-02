#pragma once

#include "OCCT.Includes.h"

/// <summary>
/// C++/CLI 对外代理类。
/// 本类保持现有 .NET 公共接口不变，成员函数实现分布在 Proxy/*.cpp 中。
/// </summary>
public ref class OCCTProxy
{
public:
    // Viewer
    bool InitViewer(System::IntPtr theWnd);
    void RedrawView(void);
    void UpdateView(void);
    void Zoom(int theX1, int theY1, int theX2, int theY2);
    void Pan(int theX, int theY);
    void Rotation(int theX, int theY);
    void StartRotation(int theX, int theY);
    void UpdateCurrentViewer(void);
    void ZoomAllView(void);
    void Reset(void);
    void ApplyDrawerHighQuality(const Handle(Prs3d_Drawer)& theDrawer);
    void ApplyAISObjectHighQuality(const Handle(AIS_InteractiveObject)& theObj);
    Standard_Real ComputeDisplayMeshDeflection(const TopoDS_Shape& theShape);
    Handle(AIS_Shape) MakeHighQualityAISShape(const TopoDS_Shape& theShape);
    void RefreshAllAISObjectsHighQuality();
    void ApplyOCCTHighQualityDisplay();

    // Selection
    void Select(void);
    void MoveTo(int theX, int theY);
    void ShiftSelect(void);
    void ClearSelected(void);
    System::Collections::Generic::List<int>^ GetSelectedIds();
    void AddToSelection(int id);

    // Visibility and lifecycle
    void SetShapeVisibility(int id, bool isVisible);
    void ClearAll(void);

    // Basic geometry
    void CreateLine(
        int id,
        double X1,
        double X2,
        double Y1,
        double Y2,
        double Z1,
        double Z2);
    void InitOCCTProxy(void);
    void CreateArcByCenter(
        int id,
        double cx,
        double cy,
        double cz,
        double radius,
        double startAngle,
        double endAngle);
    void CreatePolyline(int id, array<double>^ coords, bool isClosed);
    bool CreateCircle(
        int id,
        double cx,
        double cy,
        double cz,
        double radius);

    // Curve edit
    array<int>^ AutoCombineLines(int startId, int newId);
    bool CombineLines(int id1, int id2, int newId);
    array<int>^ SplitLine(
        int targetId,
        int toolId,
        int startingNewId,
        double geometryTolerance);
    bool DeleteLine(int id);

    // Face and cord
    bool CreateFaceFromLines(
        array<int>^ lineIds,
        int newFaceId,
        int r,
        int g,
        int b,
        double geometryTolerance);
    bool CreateCordFromTwoCurves(int id1, int id2, int newId);

    // Parametric tire section
    bool PreviewParametricTire(
        array<double>^ pointTable,
        array<int>^ segmentTable,
        array<double>^ thicknessTable,
        array<double>^ adjustmentTable,
        bool buildFace,
        bool mirror,
        double tolerance);
    array<int>^ BuildParametricTire(
        array<double>^ pointTable,
        array<int>^ segmentTable,
        array<double>^ thicknessTable,
        array<double>^ adjustmentTable,
        bool buildFace,
        bool mirror,
        int startingId,
        double tolerance);
    void CancelParametricTirePreview();
    System::String^ GetLastParametricTireError();

    // Topology edit
    void ClearTopologyEdgeDisplays();
    void DeactivateNormalShapeSelectionOnly();
    void RestoreNormalShapeSelection();
    bool RebuildTopologyEdgeDisplays(double tolerance);
    bool BeginTopologyEdit(array<int>^ componentShapeIds, double tolerance);
    array<int>^ MakeTopologyResult(
        int status,
        int keptShapeId,
        int removedShapeId);
    array<int>^ SewSelectedTopologyEdge(double tolerance);
    void FinishTopologyEdit();
    void CancelTopologyEdit();

    // CAD-style straight-line drawing on the XOY plane
    bool BeginLineCreate(double tolerance);
    bool UpdateLineCreatePreview(int theX, int theY);
    array<int>^ AddLineCreatePoint(
        int theX,
        int theY,
        int newShapeId);
    bool CancelLineCreateLine();
    void FinishLineCreate();

    // STEP/XCAF export
    bool ExportVisibleToSTEP(
        System::String^ filePath,
        array<System::String^>^ componentNames,
        array<array<int>^>^ componentShapeIds);

private:
    NCollection_Haft<Handle(V3d_Viewer)> myViewer;
    NCollection_Haft<Handle(V3d_View)> myView;
    NCollection_Haft<Handle(AIS_InteractiveContext)> myAISContext;
    NCollection_Haft<Handle(OpenGl_GraphicDriver)> myGraphicDriver;
};
