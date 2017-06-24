#include "common/const.h"
#include "object/cylinder.h"
#include "object/rotationbody.h"
#include "util/polynomial6.h"

RotationBody::RotationBody(const Vector3& o, const Curves& curves, const Material* m)
    : Object(m), m_o(o), m_curves(curves),
      m_r(0), m_h(0), m_arg(0), m_texture_ratios()
{
    for (size_t i = 0; i < m_curves.size(); i++)
        m_texture_ratios.push_back(1.0 / m_curves.size());
    m_init();
}

RotationBody::RotationBody(const Json::Value& object)
    : Object(object), m_o(object["o"]), m_curves(),
      m_r(0), m_h(0),
      m_arg(fmod(object["texture_arg"].asDouble() / 180 * Const::PI, 2 * Const::PI)), m_texture_ratios()
{
    for (auto c : object["curves"])
        m_curves.push_back(BezierCurve3(c));
    if (object["texture_ratios"].isArray())
    {
        for (auto r : object["texture_ratios"])
            m_texture_ratios.push_back(r.asDouble());
    }
    else
    {
        for (size_t i = 0; i < m_curves.size(); i++)
            m_texture_ratios.push_back(1.0 / m_curves.size());
    }
    m_init();
}

RotationBody::~RotationBody()
{
    if (m_bounding_cylinder) delete m_bounding_cylinder;
    for (auto c : m_sub_cylinders) delete c;
    m_sub_cylinders.clear();
}

Collision RotationBody::collide(const Ray& ray) const
{
    Ray uray = ray.unitize();
    Vector3 d = uray.dir;
    int curve_id = 0;
    Vector2 res(1e9, 0);

    Collision coll = m_bounding_cylinder->collide(ray);
    if (!coll.isHit()) return Collision();

    for (size_t i = 0; i < m_curves.size(); i++)
    {
        coll = m_sub_cylinders[i]->collide(ray);
        if (!coll.isHit() || (!coll.is_internal && coll.dist > res.x - Const::EPS)) continue;

        Vector3 o = ray.start - m_o;
        Vector2 w = d.toVector2(), q0, q1, q2, q3;
        // A.y^2 + B.y + C + D.x^2 = 0
        long double A = w.mod2(), B = 2 * w.dot(o.toVector2()) * d.z - 2 * o.z * A,
                    C = Vector2(o.z * d.x - o.x * d.z, o.z * d.y - o.y * d.z).mod2(), D = -d.z * d.z,
                    a[7];

        // a0 + a1.u + a2.u^2 + a3.u^3 + a4.u^4 + a5.u^5 + a6.u^6 = 0
        m_curves[i].getEquation(q0, q1, q2, q3);
        a[0] = A * q0.y * q0.y + D * q0.x * q0.x + C + B * q0.y;
        a[1] = 2 * A * q0.y * q1.y + 2 * D * q0.x * q1.x + B * q1.y;
        a[2] = A * (q1.y * q1.y + 2 * q0.y * q2.y) + D * (q1.x * q1.x + 2 * q0.x * q2.x) + B * q2.y;
        a[3] = 2 * A * (q0.y * q3.y + q1.y * q2.y) + 2 * D * (q0.x * q3.x + q1.x * q2.x) + B * q3.y;
        a[4] = A * (2 * q1.y * q3.y + q2.y * q2.y) + D * (2 * q1.x * q3.x + q2.x * q2.x);
        a[5] = 2 * (A * q2.y * q3.y + D * q2.x * q3.x);
        a[6] = A * q3.y * q3.y + D * q3.x * q3.x;

        Polynomial6 poly(a);
        std::vector<double> roots = poly.findAllRoots(0, 1);
        for (auto u : roots)
        {
            double t = -1;
            Vector2 p = m_curves[i].P(u);
            if (std::abs(d.z) > Const::EPS)
                t = (p.y - o.z) / d.z;
            else
            {
                Vector2 oc = -o.toVector2();
                double tca = oc.dot(w), thc2 = p.x * p.x - oc.mod2() + tca * tca;
                if (thc2 > -Const::EPS)
                {
                    double thc = sqrt(thc2), A = tca - thc, B = tca + thc;
                    if (A > Const::EPS)
                        t = A;
                    else
                        t = B;
                }
            }
            if (t > 1e-4 && t < res.x - Const::EPS)
                res = Vector2(t, u), curve_id = i;
        }
    }
    if (res.x < 1e9)
    {
        Vector2 v = (uray.get(res.x) - m_o).toVector2().unitize(), dp = m_curves[curve_id].dP(res.y);
        Vector3 n = Vector3(dp.y * v.x, dp.y * v.y, -dp.x);
        if (n.dot(d) < Const::EPS)
            return Collision(uray, res.x, curve_id + res.y, fmod(v.arg(), 2 * Const::PI), n, this, false, m_identifiers[curve_id]);
        else
            return Collision(uray, res.x, curve_id + res.y, fmod(v.arg(), 2 * Const::PI), -n, this, true, m_identifiers[curve_id]);
    }
    else
        return Collision();
}

Color RotationBody::getTextureColor(const Collision& coll) const
{
    if (m_material->hasTexture())
    {
        int id = coll.u;
        double u = fmod(coll.v - m_arg + 4 * Const::PI, 2 * Const::PI) / 2 / Const::PI,
               v = 1 - (m_texture_ratios_sum[id] + (coll.u - id) * m_texture_ratios[id]);
        return m_material->getTextureColor(u, v);
    }
    else
        return Color(1, 1, 1);
}

Vector3 RotationBody::P(int i, double u, double v) const
{
    Vector2 p = m_curves[i].P(u);
    return Vector3(p.x * cos(v), p.x * sin(v), p.y) + m_o;
}

void RotationBody::saveOBJ(const std::string& file, int density) const
{
    std::vector<Vector3> points;
    std::vector<std::vector<int>> meshes;
    double du = 1.0 / density, dv = 2 * Const::PI / density;
    int n = 0;
    for (size_t t = 0; t < m_curves.size(); t++)
        for (int i = 0; i <= density; i++)
            for (int j = 0; j <= density; j++)
            {
                double u = i * du, v = j * dv;
                n++;
                points.push_back(P(t, u, v));
                if (i && j) meshes.push_back({n - density - 2, n - density - 1, n, n - 1});
            }

    FILE* f = fopen(file.c_str(), "w");
    for (auto p : points) fprintf(f, "v %lf %lf %lf\n", p.x, p.y, p.z);
    for (auto m : meshes) fprintf(f, "f %d %d %d %d\n", m[0], m[1], m[2], m[3]);
    fclose(f);
}

Json::Value RotationBody::toJson() const
{
    Json::Value object = Object::toJson();
    Json::Value curves, ratios;
    for (auto c : m_curves) curves.append(c.toJson());
    for (auto r : m_texture_ratios) ratios.append(r);
    object["type"] = "RotationBody";
    object["o"] = m_o.toJson();
    object["curves"] = curves;
    object["texture_ratios"] = ratios;
    if (m_material->hasTexture()) object["texture_arg"] = m_arg * 180 / Const::PI;
    return object;
}

void RotationBody::m_init()
{
    for (auto c : m_curves)
    {
        double r = std::max(std::abs(c.L), std::abs(c.R));
        m_r = std::max(m_r, r), m_h = std::max(m_h, c.U);
        m_sub_cylinders.push_back(new Cylinder(Vector3(m_o.x, m_o.y, m_o.z + c.D), r, c.U - c.D));
        m_identifiers.push_back(Const::randUInt64());
    }
    m_bounding_cylinder = new Cylinder(m_o, m_r, m_h);

    double s = 0;
    m_texture_ratios_sum.clear();
    for (double r : m_texture_ratios)
        m_texture_ratios_sum.push_back(s), s += r;
}

Vector3 RotationBody::m_dPdu(int i, double u, double v) const
{
    Vector2 p = m_curves[i].dP(u);
    return Vector3(p.x * cos(v), p.x * sin(v), p.y);
}

Vector3 RotationBody::m_dPdv(int i, double u, double v) const
{
    double x = m_curves[i].P(u).x;
    return Vector3(-x * sin(v), x * cos(v), 0);
}
