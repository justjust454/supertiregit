#include "../OCCTProxy.h"
#include "../OCCTState.h"

#pragma region 01_Viewer_Initialization_Interaction_And_DisplayQuality
bool OCCTProxy::InitViewer(System::IntPtr theWnd)
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

/// <summary>
/// Redraw the current OCCT view.
/// </summary>
void OCCTProxy::RedrawView(void)
{
    if (!myView().IsNull())
    {
        myView()->Redraw();
    }
}

/// <summary>
/// Notify OCCT that the host window size has changed.
/// </summary>
void OCCTProxy::UpdateView(void)
{
    if (!myView().IsNull())
    {
        myView()->MustBeResized();
    }
}

/// <summary>
   ///Set Zoom
   /// </summary>
void OCCTProxy::Zoom(int theX1, int theY1, int theX2, int theY2)
{
    if (!myView().IsNull())
    {
        myView()->Zoom(theX1, theY1, theX2, theY2);
    }
}

/// <summary>
   ///Set Pan
   /// </summary>
void OCCTProxy::Pan(int theX, int theY)
{
    if (!myView().IsNull())
    {
        myView()->Pan(theX, theY);
    }
}

/// <summary>
///Rotation
/// </summary>
void OCCTProxy::Rotation(int theX, int theY)
{
    if (!myView().IsNull())
    {
        myView()->Rotation(theX, theY);
    }
}

/// <summary>
///Start rotation
/// </summary>
void OCCTProxy::StartRotation(int theX, int theY)
{
    if (!myView().IsNull())
    {
        myView()->StartRotation(theX, theY);
    }
}

/// <summary>
   ///Update current viewer
   /// </summary>
void OCCTProxy::UpdateCurrentViewer(void)
{
    if (!myAISContext().IsNull())
    {
        myAISContext()->UpdateCurrentViewer();
    }
}

/// <summary>
///Zoom in all view
/// </summary>
void OCCTProxy::ZoomAllView(void)
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
void OCCTProxy::Reset(void)
{
    if (!myView().IsNull())
    {
        myView()->Reset();
    }
}


// ============================================================
// OCC 显示质量增强：全局 Drawer 参数
// ============================================================
void OCCTProxy::ApplyDrawerHighQuality(const Handle(Prs3d_Drawer)& theDrawer)
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
void OCCTProxy::ApplyAISObjectHighQuality(const Handle(AIS_InteractiveObject)& theObj)
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
Standard_Real OCCTProxy::ComputeDisplayMeshDeflection(const TopoDS_Shape& theShape)
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
Handle(AIS_Shape) OCCTProxy::MakeHighQualityAISShape(const TopoDS_Shape& theShape)
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
void OCCTProxy::RefreshAllAISObjectsHighQuality()
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
void OCCTProxy::ApplyOCCTHighQualityDisplay()
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