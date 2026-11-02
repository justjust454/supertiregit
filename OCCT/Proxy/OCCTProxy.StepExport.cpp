#include "../OCCTProxy.h"
#include "../OCCTState.h"

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

#pragma region Proxy_STEP_XCAF_Export
    //将界面显示的内容导出为STP文件
    bool OCCTProxy::ExportVisibleToSTEP(System::String^ filePath,array<System::String^>^ componentNames,array<array<int>^>^ componentShapeIds)
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

#pragma endregion // Proxy_STEP_XCAF_Export
