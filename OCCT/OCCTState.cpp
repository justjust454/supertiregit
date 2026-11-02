#include "OCCTState.h"

std::map<int, Handle(AIS_InteractiveObject)>
    GlobalShapeMap;

std::vector<TopologyEdgeDisplayRecord>
    GlobalTopologyEdgeRecords;

std::vector<int>
    GlobalTopologyComponentIds;

std::map<int, Handle(AIS_InteractiveObject)>
    GlobalTopologyBackupObjects;

std::map<int, Standard_Boolean>
    GlobalTopologyBackupDisplayed;

Standard_Boolean GlobalTopologyEditActive =
    Standard_False;

Standard_Boolean GlobalLineCreateActive =
    Standard_False;

Standard_Boolean GlobalLineCreateHasFirstPoint =
    Standard_False;

Standard_Boolean GlobalLineCreateCursorSnapped =
    Standard_False;

gp_Pln GlobalLineCreatePlane;

gp_Pnt GlobalLineCreateFirstPoint;

Standard_Real GlobalLineCreateTolerance = 0.01;

Handle(AIS_Point) GlobalLineCreateFirstMarker;

Handle(AIS_Point) GlobalLineCreateCursorMarker;

Handle(AIS_Shape) GlobalLineCreatePreviewLine;
