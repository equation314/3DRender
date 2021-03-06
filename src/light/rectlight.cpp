#include "common/config.h"
#include "common/const.h"
#include "light/rectlight.h"
#include "object/object.h"
#include "scene/scene.h"

Collision RectLight::collide(const Ray& ray) const
{
    Vector3 n = m_dx * m_dy;
    double d = n.dot(ray.dir);
    if (std::abs(d) < Const::EPS) return Collision();
    double t = (n.dot(m_o) - n.dot(ray.start)) / d;
    if (t < Const::EPS) return Collision();
    Vector3 p = ray.get(t) - m_o;
    if (std::abs(p.dot(m_dx)) + Const::EPS < m_dx.mod2() && std::abs(p.dot(m_dy)) + Const::EPS < m_dy.mod2())
        return Collision(ray, t, this);
    else
        return Collision();
}

double RectLight::getShadowRatio(const Scene* scene, const Vector3& p) const
{
    int samples = Config::soft_shadow_samples, ret = samples * samples;
    for (int i = 0; i < samples; i++)
        for (int j = 0; j < samples; j++)
        {
            double x = (i + 0.5) * 2 / samples - 1,
                   y = (j + 0.5) * 2 / samples - 1;
            Vector3 c = m_o + m_dx * x + m_dy * y, dir = c - p;
            double dist = dir.mod();

            for (auto o = scene->objectsBegin(); o != scene->objectsEnd(); o++)
            {
                Collision coll = (*o)->collide(Ray(p, dir));
                if (coll.isHit() && coll.dist + Const::EPS < dist)
                {
                    ret--;
                    break;
                }
            }
        }
    return 1.0 * ret / samples / samples;
}

Photon RectLight::emitPhoton(double power) const
{
    return Photon(m_o + m_dx * (2 * Const::randDouble() - 1) + m_dy * (2 * Const::randDouble() - 1),
                  m_n.diffuse(), m_color * power);
}

Json::Value RectLight::toJson() const
{
    Json::Value light = Light::toJson();
    light["type"] = "RectLight";
    light["o"] = m_o.toJson();
    light["n"] = m_n.toJson();
    light["dx"] = m_dx.toJson();
    light["dy"] = m_dy.toJson();
    return light;
}
