#include "common/const.h"
#include "light/rectlight.h"
#include "object/object.h"
#include "scene/scene.h"

const int SHADOW_SAMPLE = 4;

Collision RectLight::collide(const Vector3& start, const Vector3& dir) const
{
    Vector3 n = m_dx * m_dy;
    double d = n.dot(dir);
    if (abs(d) < Const::EPS) return Collision();
    double t = (n.dot(m_o) - n.dot(start)) / d;
    if (t < Const::EPS) return Collision();
    Vector3 p = start + dir * t - m_o;
    if (abs(p.dot(m_dx)) + Const::EPS < m_dx.mod2() && abs(p.dot(m_dy)) + Const::EPS < m_dy.mod2())
        return Collision(start, dir, t, this);
    else
        return Collision();
}

double RectLight::getShadowRatio(const Scene* scene, const Vector3& p) const
{
    int ret = SHADOW_SAMPLE * SHADOW_SAMPLE;
    for (int i = 0; i < SHADOW_SAMPLE; i++)
        for (int j = 0; j < SHADOW_SAMPLE; j++)
        {
            double x = (i + 0.5) * 2 / SHADOW_SAMPLE - 1,
                   y = (j + 0.5) * 2 / SHADOW_SAMPLE - 1;
            Vector3 c = m_o + m_dx * x + m_dy * y, dir = c - p;
            double dist = dir.mod();

            for (auto o = scene->objectsBegin(); o != scene->objectsEnd(); o++)
            {
                Collision coll = (*o)->collide(p, dir);
                if (coll.isHit() && coll.dist + Const::EPS < dist)
                {
                    ret--;
                    break;
                }
            }
        }
    return 1.0 * ret / SHADOW_SAMPLE / SHADOW_SAMPLE;
}

Photon RectLight::emitPhoton(double power) const
{
    return Photon(m_o + m_dx * (2 * Const::randDouble() - 1) + m_dy * (2 * Const::randDouble() - 1),
                  Vector3::randVector(), m_color * (power / m_color.power()));
}

Json::Value RectLight::toJson() const
{
    Json::Value light = Light::toJson();
    light["type"] = "RectLight";
    light["o"] = m_o.toJson();
    light["dx"] = m_dx.toJson();
    light["dy"] = m_dy.toJson();
    return light;
}
