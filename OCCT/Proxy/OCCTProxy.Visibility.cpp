#include "../OCCTProxy.h"
#include "../OCCTState.h"

#pragma region Proxy_Visibility_And_Lifecycle
    /// <summary>
    /// 根据 ID 隐藏或显示指定的图元
    /// </summary>
    void OCCTProxy::SetShapeVisibility(int id, bool isVisible)
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
    void OCCTProxy::ClearAll(void)
    {
        if (myAISContext().IsNull()) return;
        for (auto it = GlobalShapeMap.begin(); it != GlobalShapeMap.end(); ++it)
        {
            myAISContext()->Remove(it->second, Standard_False);
        }
        GlobalShapeMap.clear();
        GlobalTopologyEdgeRecords.clear();
        GlobalTopologyComponentIds.clear();
        GlobalTopologyBackupObjects.clear();
        GlobalTopologyBackupDisplayed.clear();
        GlobalTopologyEditActive = Standard_False;
        myAISContext()->UpdateCurrentViewer();
    }

#pragma endregion // Proxy_Visibility_And_Lifecycle
