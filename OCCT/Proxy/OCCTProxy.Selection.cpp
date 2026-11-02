#include "../OCCTProxy.h"
#include "../OCCTState.h"

#pragma region Proxy_Selection
    /// <summary>
    ///Select by click
    /// </summary>
    void OCCTProxy::Select(void)
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
    void OCCTProxy::MoveTo(int theX, int theY)
    {
        if ((!myAISContext().IsNull()) && (!myView().IsNull()))
        {
            myAISContext()->MoveTo(theX, theY, myView(), Standard_True);
        }
    }

    /// <summary>
    ///Select by rectangle with pressed "Shift" key
    /// </summary>


    /// <summary>
    ///Select by "Shift" key
    /// </summary>
    void OCCTProxy::ShiftSelect(void)
    {
        if (!myAISContext().IsNull())
        {
            myAISContext()->SelectDetected(AIS_SelectionScheme_XOR);
            myAISContext()->UpdateCurrentViewer();
        }
    }

    /// <summary>
    /// 清除当前 3D 视口中所有的选中高亮状态 (供 C# 端调用)
    /// </summary>
    void OCCTProxy::ClearSelected(void)
    {
        if (!myAISContext().IsNull())
        {
            myAISContext()->ClearSelected(Standard_False); // 清空底层选择集
            myAISContext()->UpdateCurrentViewer();         // 强制刷新画面，让高亮立刻消失
        }
    }
    /// <summary>
    /// 获取当前在 3D 视口中被选中的图形的 ID 列表 (供 C# 端调用)
    /// </summary>
    System::Collections::Generic::List<int>^ OCCTProxy::GetSelectedIds()
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
    void OCCTProxy::AddToSelection(int id)
    {
        if (myAISContext().IsNull()) return;
        if (GlobalShapeMap.find(id) != GlobalShapeMap.end()) {
            // AddOrRemoveSelected 会反转选中状态，前提是先 ClearSelected
            myAISContext()->AddOrRemoveSelected(GlobalShapeMap[id], Standard_False);
        }
    }
#pragma endregion // Proxy_Selection
