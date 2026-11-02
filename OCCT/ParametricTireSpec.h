#pragma once

#include "OCCT.Includes.h"

namespace ParametricTire
{
    struct PointSpec
    {
        gp_Pnt Base;
        Standard_Real DeltaX;
        Standard_Real DeltaY;
        Standard_Boolean Locked;
    };

    struct SegmentSpec
    {
        Standard_Integer Kind;       // 0 = line, 1 = three-point arc
        Standard_Integer StartIndex;
        Standard_Integer MiddleIndex;
        Standard_Integer EndIndex;
    };

    struct ThicknessSpec
    {
        Standard_Real Tread;
        Standard_Real Buffer;
        Standard_Real Carcass;
        Standard_Real InnerLiner;
        Standard_Real Sidewall;
        Standard_Real BeadHeel;
        Standard_Real BeadCenterX;
        Standard_Real BeadCenterY;
        Standard_Real BeadRadius;
    };

    struct AdjustmentSpec
    {
        Standard_Real CrownDx;
        Standard_Real CrownDy;
        Standard_Real ShoulderDx;
        Standard_Real ShoulderDy;
        Standard_Real SidewallDx;
        Standard_Real SidewallDy;
        Standard_Real BeadDx;
        Standard_Real BeadDy;
        Standard_Real CrownHeight;
        Standard_Real HalfWidth;
    };

    enum ShapeRole
    {
        ShapeRoleOuter = 0,
        ShapeRoleInnerLiner,
        ShapeRoleCarcass,
        ShapeRoleBufferTread,
        ShapeRoleSidewall,
        ShapeRoleBeadHeel,
        ShapeRoleBeadCircle,
        ShapeRoleApex,
        ShapeRoleFace
    };

    struct StagedShape
    {
        TopoDS_Shape Shape;
        ShapeRole Role;
        Standard_Integer Red;
        Standard_Integer Green;
        Standard_Integer Blue;
        Standard_Real LineWidth;
        Standard_Boolean Shaded;

        StagedShape()
            : Role(ShapeRoleOuter),
              Red(0),
              Green(0),
              Blue(0),
              LineWidth(1.5),
              Shaded(Standard_False)
        {
        }
    };

    struct BuildOptions
    {
        Standard_Boolean BuildFace;
        Standard_Boolean Mirror;
        Standard_Real Tolerance;
    };

    struct BuildData
    {
        std::vector<PointSpec> Points;
        std::vector<SegmentSpec> Segments;
        ThicknessSpec Thickness;
        AdjustmentSpec Adjustment;
        BuildOptions Options;
    };

    struct BuildResult
    {
        std::vector<StagedShape> Shapes;
    };
}
