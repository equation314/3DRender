#include "common/const.h"
#include "object/sphere.h"

Sphere::Sphere(const Vector3& o, double r, const Material* m)
    : Object(m), m_o(o), m_r(r),
      m_dz(0, 0, 1), m_dx(1, 0, 0)
{
}

Sphere::Sphere(const Json::Value& object)
    : Object(object), m_o(object["o"]), m_r(object["r"].asDouble()),
      m_dz(object["texture_dz"]), m_dx(object["texture_dx"])
{
}

Collision Sphere::collide(const Vector3& start, const Vector3& dir) const
{
    Vector3 d = dir.unitize();
    Vector3 oc = m_o - start;
    double tca = oc.dot(d);
    if (tca > Const::EPS)
    {
        double thc2 = m_r * m_r - oc.mod2() + tca * tca;
        if (thc2 > Const::EPS)
        {
            double thc = sqrt(thc2), t1 = tca - thc, t2 = tca + thc;
            if (t1 > Const::EPS)
            {
                Vector3 p = start + d * t1;
                return Collision(start, d, t1, p - m_o, false, this);
            }
            else if (t2 > Const::EPS)
            {
                Vector3 p = start + d * t2;
                return Collision(start, d, t2, m_o - p, true, this);
            }
        }
    }

    return Collision();
}

Color Sphere::getTextureColor(const Vector3& p) const
{
    if (m_material->hasTexture())
    {
        Vector3 n = (p - m_o).unitize();
        double b = acos(n.dot(m_dz)), a = acos(std::min(std::max(n.dot(m_dx) / sin(b), -1.0), 1.0));
        double v = b / Const::PI, u = a / 2 / Const::PI;
        if (n.dot(m_dz * m_dx) < 0) u = 1 - u;
        return m_material->getTextureColor(u, v);
    }
    else
        return Color(1, 1, 1);
}

Json::Value Sphere::toJson() const
{
    Json::Value object = Object::toJson();
    object["type"] = "Sphere";
    object["o"] = m_o.toJson();
    object["r"] = m_r;
    if (m_material->hasTexture())
    {
        object["texture_dz"] = m_dz.toJson();
        object["texture_dx"] = m_dx.toJson();
    }
    return object;
}
