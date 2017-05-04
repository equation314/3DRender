#include "light/pointlight.h"

Collision PointLight::collide(const Vector3& start, const Vector3& dir) const
{
    return Collision(); // 点光源永远不会与视线相交
}
