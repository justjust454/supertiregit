#include "../OCCTProxy.h"
#include "../OCCTState.h"
#include "../Internal/OCCT.LineCreateHelpers.inl"

#pragma region Proxy_Basic_Geometry
    

    //线创建函数改造 国延林 2026.6.18
    void OCCTProxy::CreateLine(int id, double X1, double X2, double Y1, double Y2, double Z1, double Z2)
    {
        gp_Pnt aPnt1(X1, Y1, Z1);
        gp_Pnt aPnt2(X2, Y2, Z2);
        TopoDS_Shape aShape = BRepBuilderAPI_MakeEdge(aPnt1, aPnt2);

        Handle(AIS_Shape) aisShape = new AIS_Shape(aShape);

        // 【修改点 2】：把生成的对象以 id 为钥匙锁进档案室
        GlobalShapeMap[id] = aisShape;

        myAISContext()->Display(aisShape, Standard_False);
    }



    //初始化OCC，杨宇昂添加，2024.6
    void OCCTProxy::InitOCCTProxy(void)
    {
        myGraphicDriver() = NULL;
        myViewer() = NULL;
        myView() = NULL;
        myAISContext() = NULL;
    }


    //创建圆弧函数，对接DXF解析与交互绘图， 国延林，2026.06 
    void OCCTProxy::CreateArcByCenter(int id, double cx, double cy, double cz, double radius, double startAngle, double endAngle)
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
    
    /// <summary>
    ///将打散后的多段线重新合成为一个整体
    /// </summary>
    void OCCTProxy::CreatePolyline(int id, array<double>^ coords, bool isClosed)
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

    // 开始直线绘制会话。
    // 绘制只发生在 XOY 平面（Z=0）上，不涉及面选择和拓扑分割。
    bool OCCTProxy::BeginLineCreate(double tolerance)
    {
        // 交互上下文负责 AIS 对象显示，视图负责屏幕坐标转换；
        // 任意一个尚未初始化时都不能进入绘制模式。
        if (myAISContext().IsNull() || myView().IsNull())
            return false;

        // 清理上一次会话可能遗留的第一点、光标点和动态预览线。
        FinishLineCreate();

        // 避免使用小于 OCC 基础精度的无效容差。
        if (tolerance <= Precision::Confusion())
            tolerance = 0.01;

        // 建立二维图纸平面：原点位于 (0,0,0)，法向量沿 Z 轴正方向。
        GlobalLineCreatePlane =
            gp_Pln(gp_Pnt(0.0, 0.0, 0.0), gp_Dir(0.0, 0.0, 1.0));

        // 初始化本次两点绘制所需的全局状态。
        GlobalLineCreateTolerance = tolerance;
        GlobalLineCreateHasFirstPoint = Standard_False;
        GlobalLineCreateCursorSnapped = Standard_False;
        GlobalLineCreateActive = Standard_True;

        // 绘制过程中左键只负责落点，因此暂时关闭普通 Shape 选择。
        // 已有几何仍保持显示，并继续作为自动吸附的候选对象。
        DeactivateNormalShapeSelectionOnly();
        myAISContext()->ClearSelected(Standard_False);
        myAISContext()->UpdateCurrentViewer();
        return true;
    }

    // 根据屏幕鼠标坐标更新吸附标记和动态预览线。
    // 本函数只操作临时 AIS 对象，不会创建永久几何。
    bool OCCTProxy::UpdateLineCreatePreview(int theX, int theY)
    {
        // 只有处于绘制会话并且 Viewer 有效时才能计算预览。
        if (!GlobalLineCreateActive ||
            myAISContext().IsNull() ||
            myView().IsNull())
        {
            return false;
        }

        // ConvertWithProj 将二维屏幕坐标转换为视线上的三维点
        // (x,y,z) 和视线投影方向 (vx,vy,vz)。
        Standard_Real x = 0.0;
        Standard_Real y = 0.0;
        Standard_Real z = 0.0;
        Standard_Real vx = 0.0;
        Standard_Real vy = 0.0;
        Standard_Real vz = 0.0;
        myView()->ConvertWithProj(theX, theY, x, y, z, vx, vy, vz);

        const gp_Vec projectionDirection(vx, vy, vz);

        // 投影方向长度接近零时无法构造有效视线。
        if (projectionDirection.SquareMagnitude() <= Precision::SquareConfusion())
            return false;

        // 构造从当前屏幕位置射向模型空间的视线，
        // 再求该视线与 XOY 绘图平面的交点。
        const gp_Lin viewRay(
            gp_Pnt(x, y, z),
            gp_Dir(projectionDirection));
        IntAna_IntConicQuad intersection(
            viewRay,
            GlobalLineCreatePlane,
            Precision::Angular(),
            GlobalLineCreateTolerance);

        // 没有得到有效交点时，当前鼠标不能转换为图纸坐标。
        if (!intersection.IsDone() || intersection.NbPoints() == 0)
        {
            GlobalLineCreateCursorSnapped = Standard_False;
            return false;
        }

        // cursorPoint 是鼠标在绘图平面上的自由坐标；
        // displayPoint 找到吸附目标后会被替换为对应的吸附坐标。
        const gp_Pnt cursorPoint = intersection.Point(1);
        gp_Pnt displayPoint = cursorPoint;

        // 吸附距离通常取 12 个屏幕像素转换后的模型距离，
        // 同时保证不小于建模容差的 5 倍。
        const Standard_Real snapDistance =
            std::max(
                std::abs(myView()->Convert(12)),
                GlobalLineCreateTolerance * 5.0);

        // 在所有可见 Shape 中寻找最近端点或边上的最近点。
        GlobalLineCreateCursorSnapped =
            FindLineCreateSnapPoint(
                myAISContext(),
                cursorPoint,
                snapDistance,
                displayPoint);

        // 删除上一帧的鼠标标记和预览线，但保留已确定的第一点。
        RemoveLineCreatePreview(myAISContext(), Standard_False);

        // 黄色圆十字表示已吸附，浅灰十字表示当前为自由落点。
        const Quantity_Color cursorColor = GlobalLineCreateCursorSnapped
            ? Quantity_Color(1.0, 1.0, 0.0, Quantity_TOC_RGB)
            : Quantity_Color(0.9, 0.9, 0.9, Quantity_TOC_RGB);

        // 在自由投影点或最终吸附点处显示鼠标提示标记。
        GlobalLineCreateCursorMarker =
            DisplayLineCreatePoint(
                myAISContext(),
                displayPoint,
                cursorColor,
                GlobalLineCreateCursorSnapped
                ? Aspect_TOM_O_PLUS
                : Aspect_TOM_PLUS);

        // 第一点击已经确定后，用第一点和当前鼠标点创建有限长度预览线。
        if (GlobalLineCreateHasFirstPoint &&
            GlobalLineCreateFirstPoint.Distance(displayPoint) >
            GlobalLineCreateTolerance)
        {
            BRepBuilderAPI_MakeEdge previewBuilder(
                GlobalLineCreateFirstPoint,
                displayPoint);

            // OCC 成功创建临时 Edge 后，才生成相应的 AIS 预览对象。
            if (previewBuilder.IsDone())
            {
                GlobalLineCreatePreviewLine =
                    new AIS_Shape(previewBuilder.Edge());

                // 使用橙黄色和较粗线宽区分临时预览线与永久几何。
                GlobalLineCreatePreviewLine->SetColor(
                    Quantity_Color(1.0, 0.75, 0.0, Quantity_TOC_RGB));
                GlobalLineCreatePreviewLine->SetWidth(2.0);
                myAISContext()->Display(
                    GlobalLineCreatePreviewLine,
                    Standard_False);

                // 预览线只用于视觉提示，不允许被鼠标选中。
                myAISContext()->Deactivate(GlobalLineCreatePreviewLine);
            }
        }

        // 在所有临时显示对象更新完毕后统一刷新 Viewer。
        myAISContext()->UpdateCurrentViewer();
        return true;
    }

    // 处理一次左键落点。
    // 第一次调用只记录起点；第二次调用创建永久 TopoDS_Edge。
    array<int>^ OCCTProxy::AddLineCreatePoint(
        int theX,
        int theY,
        int newShapeId)
    {
        // 状态 -1：当前没有处于直线绘制会话中。
        if (!GlobalLineCreateActive)
            return MakeLineCreateResult(-1, -1);

        // 点击时重新计算鼠标投影和吸附位置，
        // 避免使用上一次 MouseMove 保存的过期坐标。
        if (!UpdateLineCreatePreview(theX, theY) ||
            GlobalLineCreateCursorMarker.IsNull())
        {
            // 状态 0：屏幕点无法转换成有效的绘图平面坐标。
            return MakeLineCreateResult(0, -1);
        }

        // AIS 光标标记中保存的是自由投影或自动吸附后的最终坐标。
        const gp_Pnt pickedPoint =
            GlobalLineCreateCursorMarker->Component()->Pnt();

        // 尚未确定起点时，本次点击作为直线的第一点。
        if (!GlobalLineCreateHasFirstPoint)
        {
            GlobalLineCreateFirstPoint = pickedPoint;
            GlobalLineCreateHasFirstPoint = Standard_True;

            // 鼠标标记将由固定的第一点标记替代，因此先从视口中移除。
            myAISContext()->Remove(
                GlobalLineCreateCursorMarker,
                Standard_False);
            GlobalLineCreateCursorMarker.Nullify();

            // 青色圆十字表示第一点已经确定。
            GlobalLineCreateFirstMarker =
                DisplayLineCreatePoint(
                    myAISContext(),
                    GlobalLineCreateFirstPoint,
                    Quantity_Color(0.0, 1.0, 1.0, Quantity_TOC_RGB),
                    Aspect_TOM_O_PLUS);
            myAISContext()->UpdateCurrentViewer();

            // 状态 1：第一点设置成功，但尚未生成永久 Shape。
            return MakeLineCreateResult(1, -1);
        }

        // 第二点与第一点距离小于建模容差时，无法形成有效直线。
        if (GlobalLineCreateFirstPoint.Distance(pickedPoint) <=
            GlobalLineCreateTolerance)
        {
            // 状态 -2：保留第一点，让用户重新指定第二点。
            return MakeLineCreateResult(-2, -1);
        }

        // GlobalShapeMap 的键必须唯一，不能覆盖已有直线或其他几何对象。
        if (newShapeId < 0 ||
            GlobalShapeMap.find(newShapeId) != GlobalShapeMap.end())
        {
            // 状态 -4：调用方提供的 Shape ID 无效或已经被占用。
            return MakeLineCreateResult(-4, -1);
        }

        try
        {
            // 使用最终确定的两个点创建一条有限长度 TopoDS_Edge。
            BRepBuilderAPI_MakeEdge edgeBuilder(
                GlobalLineCreateFirstPoint,
                pickedPoint);

            // 状态 -3：OCC 未能构造有效 Edge。
            if (!edgeBuilder.IsDone())
                return MakeLineCreateResult(-3, -1);

            // 使用项目统一的高质量显示设置创建永久 AIS_Shape。
            Handle(AIS_Shape) line =
                MakeHighQualityAISShape(edgeBuilder.Edge());

            // 永久保存的手工绘制直线统一显示为红色。
            line->SetColor(
                Quantity_Color(1.0, 0.0, 0.0, Quantity_TOC_RGB));

            // 永久直线显示前，删除第一点、鼠标点和动态预览线。
            RemoveLineCreatePreview(myAISContext(), Standard_True);

            // 将直线登记到全局 Shape 表，C# 层会使用返回的 ID 加入对应图层。
            GlobalShapeMap[newShapeId] = line;
            myAISContext()->Display(line, Standard_False);

            // 绘制模式期间不允许普通选择，但新线可以立即成为下一条线的吸附目标。
            myAISContext()->Deactivate(line);

            // 一条直线完成后，恢复到等待下一条直线第一点的状态。
            GlobalLineCreateHasFirstPoint = Standard_False;
            GlobalLineCreateCursorSnapped = Standard_False;
            myAISContext()->UpdateCurrentViewer();

            // 状态 2：永久直线创建成功，并返回它的 Shape ID。
            return MakeLineCreateResult(2, newShapeId);
        }
        catch (const Standard_Failure&)
        {
            // 防止 OCC 建模异常越过 C++/CLI 边界导致界面退出。
            return MakeLineCreateResult(-3, -1);
        }
    }

    // 取消当前尚未完成的直线，但继续停留在直线绘制模式中。
    bool OCCTProxy::CancelLineCreateLine()
    {
        if (!GlobalLineCreateActive)
            return false;

        // 返回值用于区分“确实取消了待完成直线”和“当前尚未指定第一点”。
        const Standard_Boolean hadFirstPoint =
            GlobalLineCreateHasFirstPoint;

        // 清空当前两点绘制状态，并删除所有临时显示对象。
        GlobalLineCreateHasFirstPoint = Standard_False;
        GlobalLineCreateCursorSnapped = Standard_False;
        RemoveLineCreatePreview(myAISContext(), Standard_True);

        if (!myAISContext().IsNull())
            myAISContext()->UpdateCurrentViewer();

        return hadFirstPoint == Standard_True;
    }

    // 完全结束直线绘制会话。
    // 已创建的永久直线继续保留，只清除未完成预览并恢复普通选择。
    void OCCTProxy::FinishLineCreate()
    {
        if (!myAISContext().IsNull())
        {
            // 删除第一点、当前鼠标点以及动态预览线。
            RemoveLineCreatePreview(myAISContext(), Standard_True);

            // 重新激活 GlobalShapeMap 中所有可见几何对象的整体选择模式。
            RestoreNormalShapeSelection();
            myAISContext()->ClearSelected(Standard_False);
            myAISContext()->UpdateCurrentViewer();
        }

        // 将会话标志恢复到未进入绘制模式的初始状态。
        GlobalLineCreateHasFirstPoint = Standard_False;
        GlobalLineCreateCursorSnapped = Standard_False;
        GlobalLineCreateActive = Standard_False;
    }


#pragma endregion // Proxy_Basic_Geometry
