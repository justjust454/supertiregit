#include "../OCCTProxy.h"
#include "../OCCTState.h"
#include "../Internal/OCCT.CordHelpers.inl"

#pragma region Proxy_Cord_Build
    /// <summary>
    /// 基于等弧长参数化离散的双边界中位线拟合算子
    /// </summary>
    /// <param name="id1">边界曲线 A 的全局实体标识符</param>
    /// <param name="id2">边界曲线 B 的全局实体标识符</param>
    /// <param name="newId">分配予生成的帘线实体的全新标识符</param>
    /// <returns>几何拟合与拓扑注册是否成功</returns>
    bool OCCTProxy::CreateCordFromTwoCurves(int id1, int id2, int newId)
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
#pragma endregion // Proxy_Cord_Build
