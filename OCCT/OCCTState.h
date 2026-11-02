#pragma once

#include "OCCT.Includes.h"

// 拓扑编辑期间使用的临时边显示记录。
// 类型声明放在公共头文件中，使 Visibility 和 TopologyEdit 模块
// 能够访问同一份拓扑编辑状态。
struct TopologyEdgeDisplayRecord
{
    TopoDS_Edge Edge;

    // 该边属于哪些顶层 Shape。
    std::vector<int> OwnerShapeIds;

    // 使用该边的面数量：1 = 自由边，2 = 共享边。
    Standard_Integer FaceUseCount;

    // 红色自由边或绿色共享边的临时显示对象。
    Handle(AIS_Shape) DisplayObject;
};

// 以下变量只在 OCCTState.cpp 中定义一次。
// 其他模块通过 extern 声明访问同一份数据。
extern std::map<int, Handle(AIS_InteractiveObject)>
    GlobalShapeMap;

extern std::vector<TopologyEdgeDisplayRecord>
    GlobalTopologyEdgeRecords;

extern std::vector<int>
    GlobalTopologyComponentIds;

extern std::map<int, Handle(AIS_InteractiveObject)>
    GlobalTopologyBackupObjects;

extern std::map<int, Standard_Boolean>
    GlobalTopologyBackupDisplayed;

extern Standard_Boolean
    GlobalTopologyEditActive;

// CAD-style straight-line drawing session state.
extern Standard_Boolean
    GlobalLineCreateActive;

extern Standard_Boolean
    GlobalLineCreateHasFirstPoint;

extern Standard_Boolean
    GlobalLineCreateCursorSnapped;

extern gp_Pln
    GlobalLineCreatePlane;

extern gp_Pnt
    GlobalLineCreateFirstPoint;

extern Standard_Real
    GlobalLineCreateTolerance;

extern Handle(AIS_Point)
    GlobalLineCreateFirstMarker;

extern Handle(AIS_Point)
    GlobalLineCreateCursorMarker;

extern Handle(AIS_Shape)
    GlobalLineCreatePreviewLine;
