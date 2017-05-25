#include "object/rotationbody.h"

int main()
{
    RotationBody* obj = new RotationBody(nullptr, Vector3(0, 0, 0));
    obj->addCurve(BezierCurve3(Vector2(0, 0), Vector2(0, 0), Vector2(0.8, 0), Vector2(0.8, 0)));
    obj->addCurve(BezierCurve3(Vector2(0.8, 0), Vector2(0.75, 0.1), Vector2(0.7, 0.2), Vector2(0.1, 0.2)));
    obj->addCurve(BezierCurve3(Vector2(0.1, 0.3), Vector2(0.1, 0.3), Vector2(0.1, 1.8), Vector2(0.1, 1.8)));
    obj->addCurve(BezierCurve3(Vector2(0.1, 1.8), Vector2(1.5, 2.0), Vector2(0.8, 3.5), Vector2(0.7, 3.5)));
    obj->addCurve(BezierCurve3(Vector2(0.7, 3.5), Vector2(1.4, 2.1), Vector2(0.2, 1.9), Vector2(0, 1.9)));
    obj->saveOBJ("glass.obj", 100);
}
