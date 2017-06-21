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

Collision Sphere::collide(const Ray& ray) const
{
    Ray uray = ray.unitize();
    Vector3 oc = m_o - ray.start;
    double tca = oc.dot(uray.dir), thc2 = m_r * m_r - oc.mod2() + tca * tca;
    if (thc2 >= 0)
    {
        double thc = sqrt(thc2), t1 = tca - thc, t2 = tca + thc;
        if (t1 > Const::EPS)
        {
            Vector3 p = uray.get(t1);
            return Collision(uray, t1, p - m_o, this, false);
        }
        else if (t2 > Const::EPS)
        {
            Vector3 p = uray.get(t2);
            return Collision(uray, t2, m_o - p, this, true);
        }
    }

    return Collision();
}

Color Sphere::getTextureColor(const Collision& coll) const
{
    if (m_material->hasTexture())
    {
        double b = acos(coll.n.dot(m_dz)), a = acos(std::min(std::max(coll.n.dot(m_dx) / sin(b), -1.0), 1.0));
        double v = b / Const::PI, u = a / 2 / Const::PI;
        if (coll.n.dot(m_dz * m_dx) < 0) u = 1 - u;
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
