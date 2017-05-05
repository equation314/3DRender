#include "collision.h"
#include "common/const.h"
#include "light/pointlight.h"
#include "scene.h"

Collision PointLight::collide(const Vector3& start, const Vector3& dir) const
{
    return Collision(); // 点光源永远不会与视线相交
}

double PointLight::getShadeRatio(const Scene* scene, const Vector3& p) const
{
    Vector3 dir = p - m_o;
    double dist = dir.mod();
    for (auto o = scene->objectsBegin(); o != scene->objectsEnd(); o++)
    {
        Collision coll = (*o)->collide(m_o, dir);
        if (coll.isHit() && coll.dist + Const::EPS < dist) return 0;
    }
    return 1;
}
