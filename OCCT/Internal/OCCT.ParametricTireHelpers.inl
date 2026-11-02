#pragma once

#include <gp_Trsf.hxx>
#include <sstream>
#include <string>

namespace ParametricTire
{
    inline Standard_Real ClampNonNegative(Standard_Real value)
    {
        return value < 0.0 ? 0.0 : value;
    }

    inline bool IsFiniteNumber(Standard_Real value)
    {
        return value == value
            && value != std::numeric_limits<Standard_Real>::infinity()
            && value != -std::numeric_limits<Standard_Real>::infinity();
    }

    inline Standard_Real Distance2D(const gp_Pnt& a, const gp_Pnt& b)
    {
        const Standard_Real dx = a.X() - b.X();
        const Standard_Real dy = a.Y() - b.Y();
        return std::sqrt(dx * dx + dy * dy);
    }

    inline gp_Pnt PointFrom(const std::vector<PointSpec>& points, int index)
    {
        return points[static_cast<std::size_t>(index)].Base;
    }

    inline bool ApplyPointAdjustments(BuildData& data, std::string& error)
    {
        if (data.Points.size() != 30 || data.Segments.size() != 19)
        {
            error = "Parametric tire expects 30 points and 19 segments.";
            return false;
        }

        const Standard_Real crownHeight = data.Adjustment.CrownHeight;
        const Standard_Real halfWidth = data.Adjustment.HalfWidth;
        if (!IsFiniteNumber(crownHeight) || !IsFiniteNumber(halfWidth)
            || crownHeight <= 0.0 || halfWidth <= 0.0)
        {
            error = "Tire height and half-width must be positive.";
            return false;
        }

        const Standard_Real scaleX = halfWidth / 227.5;
        const Standard_Real scaleY = crownHeight / 635.0;

        const gp_Pnt crownAnchor(0.0, crownHeight, 0.0);
        const gp_Pnt shoulderAnchor(
            193.5 * scaleX, 599.3 * scaleY, 0.0);
        const gp_Pnt sidewallAnchor(
            227.5 * scaleX, 457.0 * scaleY, 0.0);
        const gp_Pnt beadAnchor(
            data.Thickness.BeadCenterX,
            data.Thickness.BeadCenterY,
            0.0);

        struct Anchor
        {
            gp_Pnt Position;
            Standard_Real Sigma;
            Standard_Real Dx;
            Standard_Real Dy;
        };

        const Anchor anchors[] = {
            { crownAnchor, 130.0, data.Adjustment.CrownDx, data.Adjustment.CrownDy },
            { shoulderAnchor, 95.0, data.Adjustment.ShoulderDx, data.Adjustment.ShoulderDy },
            { sidewallAnchor, 100.0, data.Adjustment.SidewallDx, data.Adjustment.SidewallDy },
            { beadAnchor, 70.0, data.Adjustment.BeadDx, data.Adjustment.BeadDy }
        };

        for (std::size_t i = 0; i < data.Points.size(); ++i)
        {
            PointSpec& point = data.Points[i];
            if (!IsFiniteNumber(point.Base.X())
                || !IsFiniteNumber(point.Base.Y())
                || !IsFiniteNumber(point.DeltaX)
                || !IsFiniteNumber(point.DeltaY))
            {
                error = "A point contains an invalid numeric value.";
                return false;
            }

            gp_Pnt adjusted(
                point.Base.X() * scaleX,
                point.Base.Y() * scaleY,
                0.0);

            if (!point.Locked)
            {
                Standard_Real totalWeight = 0.0;
                Standard_Real deltaX = 0.0;
                Standard_Real deltaY = 0.0;

                for (const Anchor& anchor : anchors)
                {
                    const Standard_Real distance = Distance2D(
                        adjusted, anchor.Position);
                    const Standard_Real weight = std::exp(
                        -(distance * distance)
                        / (2.0 * anchor.Sigma * anchor.Sigma));
                    totalWeight += weight;
                    deltaX += weight * anchor.Dx;
                    deltaY += weight * anchor.Dy;
                }

                const Standard_Real denominator =
                    totalWeight > 1.0 ? totalWeight : 1.0;
                adjusted.SetX(adjusted.X() + deltaX / denominator + point.DeltaX);
                adjusted.SetY(adjusted.Y() + deltaY / denominator + point.DeltaY);
            }

            point.Base = adjusted;
        }

        return true;
    }

    inline bool ValidateSegments(
        const BuildData& data,
        std::string& error)
    {
        for (const SegmentSpec& segment : data.Segments)
        {
            if (segment.Kind != 0 && segment.Kind != 1)
            {
                error = "Unsupported segment type.";
                return false;
            }

            if (segment.StartIndex < 0 || segment.EndIndex < 0
                || segment.StartIndex >= static_cast<Standard_Integer>(data.Points.size())
                || segment.EndIndex >= static_cast<Standard_Integer>(data.Points.size()))
            {
                error = "A segment references an invalid point.";
                return false;
            }

            if (segment.Kind == 1
                && (segment.MiddleIndex < 0
                    || segment.MiddleIndex >= static_cast<Standard_Integer>(data.Points.size())))
            {
                error = "An arc segment is missing its middle point.";
                return false;
            }
        }
        return true;
    }

    inline bool BuildOneEdge(
        const BuildData& data,
        const SegmentSpec& segment,
        TopoDS_Edge& edge,
        std::string& error)
    {
        const gp_Pnt start = PointFrom(data.Points, segment.StartIndex);
        const gp_Pnt end = PointFrom(data.Points, segment.EndIndex);
        if (Distance2D(start, end) <= data.Options.Tolerance)
        {
            error = "A generated edge has zero length.";
            return false;
        }

        if (segment.Kind == 0)
        {
            BRepBuilderAPI_MakeEdge maker(start, end);
            if (!maker.IsDone())
            {
                error = "Failed to create a line edge.";
                return false;
            }
            edge = maker.Edge();
            return true;
        }

        const gp_Pnt middle = PointFrom(data.Points, segment.MiddleIndex);
        GC_MakeArcOfCircle arcMaker(start, middle, end);
        if (!arcMaker.IsDone())
        {
            error = "Failed to create a three-point arc. The points may be collinear.";
            return false;
        }

        BRepBuilderAPI_MakeEdge edgeMaker(arcMaker.Value());
        if (!edgeMaker.IsDone())
        {
            error = "Failed to convert an arc into an OCCT edge.";
            return false;
        }
        edge = edgeMaker.Edge();
        return true;
    }

    inline bool SampleSegment(
        const BuildData& data,
        const SegmentSpec& segment,
        int arcSamples,
        std::vector<gp_Pnt>& samples,
        std::string& error)
    {
        const gp_Pnt start = PointFrom(data.Points, segment.StartIndex);
        const gp_Pnt end = PointFrom(data.Points, segment.EndIndex);

        if (segment.Kind == 0)
        {
            samples.push_back(start);
            samples.push_back(end);
            return true;
        }

        const gp_Pnt middle = PointFrom(data.Points, segment.MiddleIndex);
        GC_MakeArcOfCircle arcMaker(start, middle, end);
        if (!arcMaker.IsDone())
        {
            error = "Failed to sample a three-point arc.";
            return false;
        }

        Handle(Geom_TrimmedCurve) arc = arcMaker.Value();
        const Standard_Real first = arc->FirstParameter();
        const Standard_Real last = arc->LastParameter();
        const int steps = arcSamples < 8 ? 8 : arcSamples;
        for (int i = 0; i <= steps; ++i)
        {
            const Standard_Real ratio =
                static_cast<Standard_Real>(i) / static_cast<Standard_Real>(steps);
            samples.push_back(arc->Value(first + (last - first) * ratio));
        }
        return true;
    }

    inline bool BuildPath(
        const BuildData& data,
        int firstSegment,
        int lastSegment,
        int arcSamples,
        std::vector<gp_Pnt>& path,
        std::string& error)
    {
        path.clear();
        if (firstSegment < 0 || lastSegment < firstSegment
            || lastSegment >= static_cast<int>(data.Segments.size()))
        {
            error = "Invalid segment range for an internal boundary.";
            return false;
        }

        for (int i = firstSegment; i <= lastSegment; ++i)
        {
            std::vector<gp_Pnt> samples;
            if (!SampleSegment(data, data.Segments[i], arcSamples, samples, error))
                return false;

            if (!path.empty())
            {
                if (Distance2D(path.back(), samples.front())
                    > data.Options.Tolerance)
                {
                    error = "Internal boundary segments are not connected.";
                    return false;
                }
                samples.erase(samples.begin());
            }
            path.insert(path.end(), samples.begin(), samples.end());
        }

        return path.size() >= 2;
    }

    inline std::vector<gp_Pnt> OffsetPath(
        const std::vector<gp_Pnt>& path,
        double distance)
    {
        std::vector<gp_Pnt> result;
        result.reserve(path.size());
        if (path.size() < 2)
            return result;

        for (std::size_t i = 0; i < path.size(); ++i)
        {
            gp_Vec tangent;
            if (i == 0)
                tangent = gp_Vec(path[1], path[0]).Reversed();
            else if (i + 1 == path.size())
                tangent = gp_Vec(path[i - 1], path[i]);
            else
                tangent = gp_Vec(path[i - 1], path[i + 1]);

            const Standard_Real magnitude = tangent.Magnitude();
            if (magnitude <= Precision::Confusion())
            {
                if (result.empty())
                    result.push_back(path[i]);
                else
                    result.push_back(result.back());
                continue;
            }

            tangent.Divide(magnitude);
            gp_Vec normal(tangent.Y(), -tangent.X(), 0.0);
            gp_Pnt offsetPoint = path[i].Translated(normal * distance);

            // Keep the symmetry-axis endpoint on X=0.
            if (std::abs(path[i].X()) <= 1e-8)
                offsetPoint.SetX(0.0);

            result.push_back(offsetPoint);
        }
        return result;
    }

    inline Standard_Real Cross2D(
        const gp_Pnt& a,
        const gp_Pnt& b,
        const gp_Pnt& c)
    {
        return (b.X() - a.X()) * (c.Y() - a.Y())
            - (b.Y() - a.Y()) * (c.X() - a.X());
    }

    inline bool SegmentsIntersectProperly(
        const gp_Pnt& a,
        const gp_Pnt& b,
        const gp_Pnt& c,
        const gp_Pnt& d,
        Standard_Real tolerance)
    {
        const Standard_Real o1 = Cross2D(a, b, c);
        const Standard_Real o2 = Cross2D(a, b, d);
        const Standard_Real o3 = Cross2D(c, d, a);
        const Standard_Real o4 = Cross2D(c, d, b);
        return o1 * o2 < -tolerance && o3 * o4 < -tolerance;
    }

    inline bool IsSimplePath(
        const std::vector<gp_Pnt>& path,
        Standard_Real tolerance)
    {
        if (path.size() < 4)
            return true;

        for (std::size_t i = 0; i + 1 < path.size(); ++i)
        {
            for (std::size_t j = i + 2; j + 1 < path.size(); ++j)
            {
                if (i == 0 && j + 2 == path.size())
                    continue; // valid closing adjacency

                if (SegmentsIntersectProperly(
                    path[i], path[i + 1], path[j], path[j + 1], tolerance))
                {
                    return false;
                }
            }
        }
        return true;
    }

    inline bool EdgeFromSamples(
        const std::vector<gp_Pnt>& samples,
        TopoDS_Edge& edge,
        std::string& error)
    {
        if (samples.size() < 3)
        {
            error = "Not enough samples to create an internal curve.";
            return false;
        }

        Handle(TColgp_HArray1OfPnt) points =
            new TColgp_HArray1OfPnt(
                1, static_cast<Standard_Integer>(samples.size()));
        for (std::size_t i = 0; i < samples.size(); ++i)
            points->SetValue(static_cast<Standard_Integer>(i + 1), samples[i]);

        GeomAPI_PointsToBSpline maker(points->Array1());
        if (!maker.IsDone())
        {
            error = "Failed to fit an internal offset curve.";
            return false;
        }

        BRepBuilderAPI_MakeEdge edgeMaker(maker.Curve());
        if (!edgeMaker.IsDone())
        {
            error = "Failed to create an OCCT edge from an internal curve.";
            return false;
        }
        edge = edgeMaker.Edge();
        return true;
    }

    inline StagedShape MakeStaged(
        const TopoDS_Shape& shape,
        ShapeRole role,
        int red,
        int green,
        int blue,
        double width,
        bool shaded)
    {
        StagedShape staged;
        staged.Shape = shape;
        staged.Role = role;
        staged.Red = red;
        staged.Green = green;
        staged.Blue = blue;
        staged.LineWidth = width;
        staged.Shaded = shaded ? Standard_True : Standard_False;
        return staged;
    }

    inline bool AddOffsetCurve(
        const std::vector<gp_Pnt>& sourcePath,
        double distance,
        ShapeRole role,
        int red,
        int green,
        int blue,
        double width,
        std::vector<StagedShape>& shapes,
        std::string& error,
        const char* label)
    {
        if (distance <= 0.0)
        {
            error = std::string(label) + " has an invalid thickness.";
            return false;
        }

        const std::vector<gp_Pnt> offset = OffsetPath(sourcePath, distance);
        if (!IsSimplePath(offset, 1e-9))
        {
            error = std::string(label)
                + " self-intersects at the selected thickness.";
            return false;
        }

        TopoDS_Edge edge;
        if (!EdgeFromSamples(offset, edge, error))
            return false;

        shapes.push_back(MakeStaged(
            edge, role, red, green, blue, width, false));
        return true;
    }

    inline bool BuildBeadTangentPoints(
        const gp_Pnt& point,
        const gp_Pnt& center,
        Standard_Real radius,
        gp_Pnt& first,
        gp_Pnt& second,
        std::string& error)
    {
        const Standard_Real dx = point.X() - center.X();
        const Standard_Real dy = point.Y() - center.Y();
        const Standard_Real distance = std::sqrt(dx * dx + dy * dy);
        if (distance <= radius + 1e-7)
        {
            error = "Bead radius is too large for the apex tangent construction.";
            return false;
        }

        const Standard_Real baseAngle = std::atan2(dy, dx);
        const Standard_Real delta = std::acos(radius / distance);
        const Standard_Real angle1 = baseAngle + delta;
        const Standard_Real angle2 = baseAngle - delta;
        first = gp_Pnt(
            center.X() + radius * std::cos(angle1),
            center.Y() + radius * std::sin(angle1),
            0.0);
        second = gp_Pnt(
            center.X() + radius * std::cos(angle2),
            center.Y() + radius * std::sin(angle2),
            0.0);
        return true;
    }

    inline bool BuildGeometry(
        BuildData data,
        BuildResult& result,
        std::string& error)
    {
        result.Shapes.clear();
        if (!ValidateSegments(data, error)
            || !ApplyPointAdjustments(data, error))
        {
            return false;
        }

        const ThicknessSpec& thickness = data.Thickness;
        if (thickness.Tread <= 0.05 || thickness.Buffer <= 0.05
            || thickness.Carcass <= 0.05 || thickness.InnerLiner <= 0.05
            || thickness.Sidewall <= 0.05 || thickness.BeadHeel <= 0.05
            || thickness.BeadRadius <= 0.05)
        {
            error = "All material thickness values must be positive.";
            return false;
        }

        std::vector<TopoDS_Edge> outerEdges;
        outerEdges.reserve(data.Segments.size());
        for (const SegmentSpec& segment : data.Segments)
        {
            TopoDS_Edge edge;
            if (!BuildOneEdge(data, segment, edge, error))
                return false;

            if (!outerEdges.empty())
            {
                const gp_Pnt previousEnd = BRep_Tool::Pnt(
                    TopExp::LastVertex(outerEdges.back(), Standard_True));
                const gp_Pnt currentStart = BRep_Tool::Pnt(
                    TopExp::FirstVertex(edge, Standard_True));
                if (Distance2D(previousEnd, currentStart)
                    > data.Options.Tolerance)
                {
                    error = "Outer contour has an endpoint gap.";
                    return false;
                }
            }

            outerEdges.push_back(edge);
            result.Shapes.push_back(MakeStaged(
                edge, ShapeRoleOuter, 25, 25, 25, 1.8, false));
        }

        std::vector<gp_Pnt> innerPath;
        if (!BuildPath(data, 12, 17, 48, innerPath, error))
            return false;
        if (!IsSimplePath(innerPath, 1e-9))
        {
            error = "The base inner contour self-intersects.";
            return false;
        }

        std::vector<gp_Pnt> upperInnerPath;
        if (!BuildPath(data, 15, 17, 48, upperInnerPath, error))
            return false;

        std::vector<gp_Pnt> lowerInnerPath;
        if (!BuildPath(data, 12, 14, 48, lowerInnerPath, error))
            return false;

        const Standard_Real linerDepth = thickness.InnerLiner;
        const Standard_Real carcassDepth =
            thickness.InnerLiner + thickness.Carcass;
        const Standard_Real bufferDepth =
            thickness.InnerLiner + thickness.Carcass + thickness.Buffer;
        const Standard_Real sidewallDepth =
            thickness.InnerLiner + thickness.Carcass + thickness.Sidewall;

        if (!AddOffsetCurve(
            innerPath, linerDepth, ShapeRoleInnerLiner,
            20, 120, 175, 1.3, result.Shapes, error, "Inner liner"))
        {
            return false;
        }
        if (!AddOffsetCurve(
            innerPath, carcassDepth, ShapeRoleCarcass,
            40, 145, 95, 1.3, result.Shapes, error, "Carcass"))
        {
            return false;
        }
        if (!AddOffsetCurve(
            upperInnerPath, bufferDepth, ShapeRoleBufferTread,
            215, 120, 25, 1.3, result.Shapes, error, "Buffer/tread boundary"))
        {
            return false;
        }
        if (!AddOffsetCurve(
            lowerInnerPath, sidewallDepth, ShapeRoleSidewall,
            150, 80, 170, 1.3, result.Shapes, error, "Sidewall"))
        {
            return false;
        }

        std::vector<gp_Pnt> heelPath;
        if (!BuildPath(data, 9, 11, 32, heelPath, error))
            return false;
        if (!AddOffsetCurve(
            heelPath, thickness.BeadHeel, ShapeRoleBeadHeel,
            45, 100, 170, 1.3, result.Shapes, error, "Bead heel"))
        {
            return false;
        }

        gp_Circ beadCircle(
            gp_Ax2(
                gp_Pnt(thickness.BeadCenterX, thickness.BeadCenterY, 0.0),
                gp_Dir(0.0, 0.0, 1.0)),
            thickness.BeadRadius);
        BRepBuilderAPI_MakeEdge beadEdgeMaker(beadCircle);
        if (!beadEdgeMaker.IsDone())
        {
            error = "Failed to create the bead circle.";
            return false;
        }
        result.Shapes.push_back(MakeStaged(
            beadEdgeMaker.Edge(), ShapeRoleBeadCircle,
            180, 80, 20, 1.6, false));

        const Standard_Real radius = thickness.BeadRadius;
        const gp_Pnt beadCenter(
            thickness.BeadCenterX,
            thickness.BeadCenterY,
            0.0);
        const gp_Pnt apexLeft(
            thickness.BeadCenterX + 0.8502994012 * radius,
            thickness.BeadCenterY + 2.0359281437 * radius,
            0.0);
        const gp_Pnt apexRight(
            thickness.BeadCenterX + radius,
            thickness.BeadCenterY + 2.0359281437 * radius,
            0.0);
        gp_Pnt tangentLeft1, tangentLeft2, tangentRight1, tangentRight2;
        if (!BuildBeadTangentPoints(
            apexLeft,
            beadCenter,
            radius,
            tangentLeft1,
            tangentLeft2,
            error)
            || !BuildBeadTangentPoints(
                apexRight,
                beadCenter,
                radius,
                tangentRight1,
                tangentRight2,
                error))
        {
            return false;
        }

        const gp_Pnt selectedLeft =
            tangentLeft1.X() < tangentLeft2.X() ? tangentLeft1 : tangentLeft2;
        const gp_Pnt selectedRight =
            tangentRight1.X() > tangentRight2.X() ? tangentRight1 : tangentRight2;

        const gp_Pnt apexPairs[3][2] = {
            { apexLeft, selectedLeft },
            { selectedLeft, selectedRight },
            { selectedRight, apexRight }
        };
        for (const auto& pair : apexPairs)
        {
            if (Distance2D(pair[0], pair[1]) <= data.Options.Tolerance)
                continue;

            BRepBuilderAPI_MakeEdge edgeMaker(pair[0], pair[1]);
            if (!edgeMaker.IsDone())
            {
                error = "Failed to create an apex tangent line.";
                return false;
            }
            result.Shapes.push_back(MakeStaged(
                edgeMaker.Edge(), ShapeRoleApex,
                95, 95, 95, 1.2, false));
        }

        if (data.Options.BuildFace)
        {
            BRepBuilderAPI_MakeWire wireMaker;
            for (const TopoDS_Edge& edge : outerEdges)
                wireMaker.Add(edge);
            if (!wireMaker.IsDone())
            {
                error = "Failed to assemble the outer profile into one closed wire.";
                return false;
            }

            TopoDS_Wire wire = wireMaker.Wire();
            if (!wire.Closed())
            {
                error = "The generated tire contour is not closed.";
                return false;
            }

            BRepBuilderAPI_MakeFace faceMaker(wire, Standard_True);
            if (!faceMaker.IsDone())
            {
                error = "Failed to create the tire section face.";
                return false;
            }

            TopoDS_Face face = faceMaker.Face();
            BRepCheck_Analyzer analyzer(face);
            if (!analyzer.IsValid())
            {
                error = "OCCT reports the generated tire face as invalid.";
                return false;
            }

            result.Shapes.push_back(MakeStaged(
                face, ShapeRoleFace, 195, 210, 225, 1.0, true));
        }

        if (data.Options.Mirror)
        {
            gp_Trsf mirror;
            mirror.SetMirror(gp_Ax2(
                gp_Pnt(0.0, 0.0, 0.0),
                gp_Dir(1.0, 0.0, 0.0)));
            const TopLoc_Location mirrorLocation(mirror);

            const std::size_t originalCount = result.Shapes.size();
            for (std::size_t i = 0; i < originalCount; ++i)
            {
                StagedShape mirroredResult = result.Shapes[i];
                mirroredResult.Shape = result.Shapes[i].Shape.Moved(mirrorLocation);
                result.Shapes.push_back(mirroredResult);
            }
        }

        return !result.Shapes.empty();
    }
}
