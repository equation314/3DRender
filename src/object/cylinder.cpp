#include "common/const.h"
#include "object/cylinder.h"

Cylinder::Cylinder(const Vector3& o, double r, double h, const Material* m)
    : Object(m), m_o(o), m_r(r), m_h(h),
      m_arg(0)
{
}

Cylinder::Cylinder(const Json::Value& object)
    : Object(object), m_o(object["o"]), m_r(object["r"].asDouble()), m_h(object["h"].asDouble()),
      m_arg(fmod(object["texture_arg"].asDouble() / 180 * Const::PI, 2 * Const::PI))
{
}

Collision Cylinder::collide(const Ray& ray) const
{
    Ray uray = ray.unitize();
    Vector3 d3 = uray.dir;
    Vector2 d2(d3.x, d3.y), oc = Vector2(m_o.x - ray.start.x, m_o.y - ray.start.y);
    double t = -1;
    bool in = m_o.z < ray.start.z + Const::EPS && ray.start.z < m_o.z + m_h + Const::EPS && oc.mod2() < m_r * m_r + Const::EPS;

    // 与两个底面求交
    if (std::abs(d3.z) > Const::EPS)
    {
        double t1 = (m_o.z - ray.start.z) / d3.z, t2 = (m_o.z + m_h - ray.start.z) / d3.z, u1 = 0, u2 = 1;
        if (t1 > t2) std::swap(t1, t2), std::swap(u1, u2);
        if (t1 > Const::EPS) // 若射线和第一个底面相交，直接返回
        {
            Vector3 p = uray.get(t1);
            Vector2 q = (p - m_o).toVector2();
            if (q.mod2() < m_r * m_r + Const::EPS)
                return Collision(uray, t1, u1, q.arg(), Vector3(0, 0, -d3.z), this, in);
        }
        else if (t2 > Const::EPS) // 若射线和第二个底面相交，且射线起点在圆柱体内，也直接返回
        {
            Vector3 p = uray.get(t2);
            Vector2 q = (p - m_o).toVector2();
            if (q.mod2() < m_r * m_r + Const::EPS && in)
                return Collision(uray, t2, u2, q.arg(), Vector3(0, 0, -d3.z), this, in);
        }
        else // 若射线不和上下底面所在的平面相交，则无交点
            return Collision();
    }

    // 若射线垂直于 xy 平面，则交点不会在圆柱面上
    if (d2.mod2() < Const::EPS && t < 0) return Collision();

    // 否则第一个交点为圆柱面
    double tca = oc.dot(d2.unitize()), thc2 = m_r * m_r - oc.mod2() + tca * tca;
    if (thc2 >= 0)
    {
        double thc = sqrt(thc2), t1 = tca - thc, t2 = tca + thc;
        if (t1 > Const::EPS)
            t = t1;
        else
            t = t2;
    }
    if (t > Const::EPS)
    {
        t /= d2.mod();
        Vector3 p = uray.get(t);
        if (m_o.z < p.z + Const::EPS && p.z < m_o.z + m_h + Const::EPS)
            return Collision(uray, t, (p.z - m_o.z) / m_h, (p - m_o).toVector2().arg(),
                             Vector3(p.x - m_o.x, p.y - m_o.y, 0) * (in ? -1 : 1), this, in);
    }
    return Collision();
}

Color Cylinder::getTextureColor(const Collision& coll) const
{
    if (m_material->hasTexture())
    {
        double u = fmod(coll.v - m_arg + 4 * Const::PI, 2 * Const::PI) / 2 / Const::PI,
               v = 1 - coll.u;
        return m_material->getTextureColor(u, v);
    }
    else
        return Color(1, 1, 1);
}

Json::Value Cylinder::toJson() const
{
    Json::Value object = Object::toJson();
    object["type"] = "Cylinder";
    object["o"] = m_o.toJson();
    object["r"] = m_r;
    object["h"] = m_h;
    if (m_material->hasTexture()) object["texture_arg"] = m_arg * 180 / Const::PI;
    return object;
}
