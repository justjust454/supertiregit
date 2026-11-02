#include "../OCCTProxy.h"
#include "../OCCTState.h"
#include "../ParametricTireSpec.h"
#include "../Internal/OCCT.ParametricTireHelpers.inl"

namespace
{
    std::string GlobalLastParametricTireError;
    std::vector<Handle(AIS_InteractiveObject)> GlobalParametricTirePreview;

    struct PreparedObject
    {
        int Id;
        Handle(AIS_Shape) Object;
    };

    bool ParseArrays(
        array<double>^ pointTable,
        array<int>^ segmentTable,
        array<double>^ thicknessTable,
        array<double>^ adjustmentTable,
        bool buildFace,
        bool mirror,
        double tolerance,
        ParametricTire::BuildData& data,
        std::string& error)
    {
        if (pointTable == nullptr || segmentTable == nullptr
            || thicknessTable == nullptr || adjustmentTable == nullptr)
        {
            error = "A parameter table is null.";
            return false;
        }

        if (pointTable->Length != 150)
        {
            error = "The point table must contain 30 points with 5 values each.";
            return false;
        }

        if (segmentTable->Length != 76)
        {
            error = "The segment table must contain 19 segments with 4 values each.";
            return false;
        }

        if (thicknessTable->Length < 9)
        {
            error = "The thickness table is incomplete.";
            return false;
        }

        if (adjustmentTable->Length < 10)
        {
            error = "The adjustment table is incomplete.";
            return false;
        }

        if (tolerance <= Precision::Confusion())
        {
            error = "The geometry tolerance must be positive.";
            return false;
        }

        data.Points.clear();
        data.Points.reserve(30);
        for (int i = 0; i < 30; ++i)
        {
            ParametricTire::PointSpec point;
            const int offset = i * 5;
            point.Base = gp_Pnt(
                pointTable[offset],
                pointTable[offset + 1],
                0.0);
            point.DeltaX = pointTable[offset + 2];
            point.DeltaY = pointTable[offset + 3];
            point.Locked = pointTable[offset + 4] >= 0.5
                ? Standard_True : Standard_False;
            data.Points.push_back(point);
        }

        data.Segments.clear();
        data.Segments.reserve(19);
        for (int i = 0; i < 19; ++i)
        {
            const int offset = i * 4;
            ParametricTire::SegmentSpec segment;
            segment.Kind = segmentTable[offset];
            segment.StartIndex = segmentTable[offset + 1];
            segment.MiddleIndex = segmentTable[offset + 2];
            segment.EndIndex = segmentTable[offset + 3];
            data.Segments.push_back(segment);
        }

        data.Thickness.Tread = thicknessTable[0];
        data.Thickness.Buffer = thicknessTable[1];
        data.Thickness.Carcass = thicknessTable[2];
        data.Thickness.InnerLiner = thicknessTable[3];
        data.Thickness.Sidewall = thicknessTable[4];
        data.Thickness.BeadHeel = thicknessTable[5];
        data.Thickness.BeadCenterX = thicknessTable[6];
        data.Thickness.BeadCenterY = thicknessTable[7];
        data.Thickness.BeadRadius = thicknessTable[8];

        data.Adjustment.CrownDx = adjustmentTable[0];
        data.Adjustment.CrownDy = adjustmentTable[1];
        data.Adjustment.ShoulderDx = adjustmentTable[2];
        data.Adjustment.ShoulderDy = adjustmentTable[3];
        data.Adjustment.SidewallDx = adjustmentTable[4];
        data.Adjustment.SidewallDy = adjustmentTable[5];
        data.Adjustment.BeadDx = adjustmentTable[6];
        data.Adjustment.BeadDy = adjustmentTable[7];
        data.Adjustment.CrownHeight = adjustmentTable[8];
        data.Adjustment.HalfWidth = adjustmentTable[9];

        data.Options.BuildFace = buildFace
            ? Standard_True : Standard_False;
        data.Options.Mirror = mirror
            ? Standard_True : Standard_False;
        data.Options.Tolerance = tolerance;
        return true;
    }

    void ConfigureAisShape(
        const Handle(AIS_Shape)& aisShape,
        const ParametricTire::StagedShape& staged)
    {
        const Quantity_Color color(
            staged.Red / 255.0,
            staged.Green / 255.0,
            staged.Blue / 255.0,
            Quantity_TOC_RGB);
        aisShape->SetColor(color);
        aisShape->SetWidth(staged.LineWidth);

        if (staged.Shaded)
        {
            aisShape->SetDisplayMode(AIS_Shaded);
            aisShape->SetTransparency(0.70);
            aisShape->Attributes()->SetFaceBoundaryDraw(Standard_True);
        }
        else
        {
            aisShape->SetDisplayMode(AIS_WireFrame);
        }
    }
}

bool OCCTProxy::PreviewParametricTire(
    array<double>^ pointTable,
    array<int>^ segmentTable,
    array<double>^ thicknessTable,
    array<double>^ adjustmentTable,
    bool buildFace,
    bool mirror,
    double tolerance)
{
    GlobalLastParametricTireError.clear();
    CancelParametricTirePreview();

    try
    {
        if (myAISContext().IsNull())
        {
            GlobalLastParametricTireError =
                "The OCCT viewer is not initialized.";
            return false;
        }

        ParametricTire::BuildData data;
        std::string error;
        if (!ParseArrays(
            pointTable,
            segmentTable,
            thicknessTable,
            adjustmentTable,
            buildFace,
            mirror,
            tolerance,
            data,
            error))
        {
            GlobalLastParametricTireError = error;
            return false;
        }

        ParametricTire::BuildResult result;
        if (!ParametricTire::BuildGeometry(data, result, error))
        {
            GlobalLastParametricTireError = error;
            return false;
        }

        for (const ParametricTire::StagedShape& staged : result.Shapes)
        {
            Handle(AIS_Shape) aisShape = MakeHighQualityAISShape(staged.Shape);
            if (aisShape.IsNull())
            {
                CancelParametricTirePreview();
                GlobalLastParametricTireError =
                    "OCCT failed to create a preview object.";
                return false;
            }

            ConfigureAisShape(aisShape, staged);
            myAISContext()->Display(aisShape, Standard_False);
            myAISContext()->Deactivate(aisShape);
            GlobalParametricTirePreview.push_back(aisShape);
        }

        myAISContext()->UpdateCurrentViewer();
        return true;
    }
    catch (const Standard_Failure&)
    {
        CancelParametricTirePreview();
        GlobalLastParametricTireError =
            "An OpenCASCADE exception occurred while building the preview.";
        return false;
    }
    catch (System::Exception^ exception)
    {
        CancelParametricTirePreview();
        GlobalLastParametricTireError =
            "A managed exception occurred while building the preview.";
        (void)exception;
        return false;
    }
    catch (...)
    {
        CancelParametricTirePreview();
        GlobalLastParametricTireError =
            "An unexpected exception occurred while building the preview.";
        return false;
    }
}

array<int>^ OCCTProxy::BuildParametricTire(
    array<double>^ pointTable,
    array<int>^ segmentTable,
    array<double>^ thicknessTable,
    array<double>^ adjustmentTable,
    bool buildFace,
    bool mirror,
    int startingId,
    double tolerance)
{
    GlobalLastParametricTireError.clear();
    CancelParametricTirePreview();

    try
    {
        if (myAISContext().IsNull())
        {
            GlobalLastParametricTireError =
                "The OCCT viewer is not initialized.";
            return gcnew array<int>(0);
        }

        if (startingId <= 0)
        {
            GlobalLastParametricTireError =
                "The starting shape id must be positive.";
            return gcnew array<int>(0);
        }

        ParametricTire::BuildData data;
        std::string error;
        if (!ParseArrays(
            pointTable,
            segmentTable,
            thicknessTable,
            adjustmentTable,
            buildFace,
            mirror,
            tolerance,
            data,
            error))
        {
            GlobalLastParametricTireError = error;
            return gcnew array<int>(0);
        }

        ParametricTire::BuildResult result;
        if (!ParametricTire::BuildGeometry(data, result, error))
        {
            GlobalLastParametricTireError = error;
            return gcnew array<int>(0);
        }

        const int count = static_cast<int>(result.Shapes.size());
        if (count <= 0
            || startingId > std::numeric_limits<int>::max() - count)
        {
            GlobalLastParametricTireError =
                "The generated shape id range is invalid.";
            return gcnew array<int>(0);
        }

        for (int i = 0; i < count; ++i)
        {
            if (GlobalShapeMap.find(startingId + i) != GlobalShapeMap.end())
            {
                GlobalLastParametricTireError =
                    "A generated shape id is already in GlobalShapeMap.";
                return gcnew array<int>(0);
            }
        }

        std::vector<PreparedObject> prepared;
        prepared.reserve(result.Shapes.size());

        for (int i = 0; i < count; ++i)
        {
            Handle(AIS_Shape) aisShape =
                MakeHighQualityAISShape(result.Shapes[i].Shape);
            if (aisShape.IsNull())
            {
                GlobalLastParametricTireError =
                    "OCCT failed to create a committed geometry object.";
                return gcnew array<int>(0);
            }

            ConfigureAisShape(aisShape, result.Shapes[i]);
            PreparedObject object;
            object.Id = startingId + i;
            object.Object = aisShape;
            prepared.push_back(object);
        }

        array<int>^ ids = gcnew array<int>(count);
        for (int i = 0; i < count; ++i)
        {
            ids[i] = prepared[i].Id;
            GlobalShapeMap[prepared[i].Id] = prepared[i].Object;
            myAISContext()->Display(prepared[i].Object, Standard_False);
        }

        myAISContext()->ClearSelected(Standard_False);
        myAISContext()->UpdateCurrentViewer();
        return ids;
    }
    catch (const Standard_Failure&)
    {
        GlobalLastParametricTireError =
            "An OpenCASCADE exception occurred while committing geometry.";
        return gcnew array<int>(0);
    }
    catch (System::Exception^ exception)
    {
        GlobalLastParametricTireError =
            "A managed exception occurred while committing geometry.";
        (void)exception;
        return gcnew array<int>(0);
    }
    catch (...)
    {
        GlobalLastParametricTireError =
            "An unexpected exception occurred while committing geometry.";
        return gcnew array<int>(0);
    }
}

void OCCTProxy::CancelParametricTirePreview()
{
    if (!myAISContext().IsNull())
    {
        for (const Handle(AIS_InteractiveObject)& object
            : GlobalParametricTirePreview)
        {
            if (!object.IsNull())
                myAISContext()->Remove(object, Standard_False);
        }
        if (!GlobalParametricTirePreview.empty())
            myAISContext()->UpdateCurrentViewer();
    }

    GlobalParametricTirePreview.clear();
}

System::String^ OCCTProxy::GetLastParametricTireError()
{
    return gcnew System::String(
        GlobalLastParametricTireError.c_str());
}
