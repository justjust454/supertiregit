#include "../OCCTProxy.h"
#include "../OCCTState.h"

bool OCCTProxy::CreateCircle(
    int id,
    double cx,
    double cy,
    double cz,
    double radius)
{
    if (myAISContext().IsNull()
        || id <= 0
        || radius <= Precision::Confusion()
        || GlobalShapeMap.find(id) != GlobalShapeMap.end())
    {
        return false;
    }

    const gp_Pnt center(cx, cy, cz);
    const gp_Dir normal(0.0, 0.0, 1.0);
    const gp_Ax2 axis(center, normal);
    const gp_Circ circle(axis, radius);

    BRepBuilderAPI_MakeEdge circleMaker(circle);
    if (!circleMaker.IsDone())
        return false;

    Handle(AIS_Shape) circleShape =
        MakeHighQualityAISShape(circleMaker.Edge());
    if (circleShape.IsNull())
        return false;

    GlobalShapeMap[id] = circleShape;
    myAISContext()->Display(circleShape, Standard_False);
    myAISContext()->ClearSelected(Standard_False);
    myAISContext()->UpdateCurrentViewer();
    return true;
}
