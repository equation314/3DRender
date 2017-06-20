#include "common/const.h"
#include "light/pointlight.h"
#include "object/object.h"
#include "scene/scene.h"

Collision PointLight::collide(const Vector3& start, const Vector3& dir) const
{
    return Collision(); // 点光源永远不会与视线相交
}

double PointLight::getShadowRatio(const Scene* scene, const Vector3& p) const
{
    Vector3 dir = m_o - p;
    double dist = dir.mod();
    for (auto o = scene->objectsBegin(); o != scene->objectsEnd(); o++)
    {
        Collision coll = (*o)->collide(p, dir);
        if (coll.isHit() && coll.dist + Const::EPS < dist) return 0;
    }
    return 1;
}

Photon PointLight::emitPhoton(double power) const
{
    return Photon(m_o, Vector3::randVector(), m_color * power);
}

Json::Value PointLight::toJson() const
{
    Json::Value light = Light::toJson();
    light["type"] = "PointLight";
    light["o"] = m_o.toJson();
    return light;
}
