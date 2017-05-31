#include "common/const.h"
#include "object/cylinder.h"
#include "object/rotationbody.h"

const int SUBSURFACE_NUM = 32;
const int ITERATION_NUM = 20;

RotationBody::RotationBody(const Vector3& o, const Curves& curves, const Material* m)
    : Object(m), m_o(o), m_curves(curves),
      m_r(0), m_h(0), m_arg(0)
{
    m_init();
}

RotationBody::RotationBody(const Json::Value& object)
    : Object(object), m_o(object["o"]), m_curves(),
      m_r(0), m_h(0), m_arg(fmod(object["texture_arg"].asDouble() / 180 * Const::PI, 2 * Const::PI))
{
    for (auto c : object["curves"])
        m_curves.push_back(BezierCurve3(c));
    m_init();
}

RotationBody::~RotationBody()
{
    if (m_bounding_cylinder) delete m_bounding_cylinder;
    for (auto c : m_sub_cylinders) delete c;
    m_sub_cylinders.clear();
}

Collision RotationBody::collide(const Vector3& start, const Vector3& dir) const
{
    Vector3 d = dir.unitize();
    int curve_id = 0;
    Vector3 res(1e9, 0, 0);

    Collision coll = m_bounding_cylinder->collide(start, d);
    if (!coll.isHit()) return Collision();

    for (size_t i = 0; i < m_curves.size(); i++)
    {
        coll = m_sub_cylinders[i]->collide(start, d);
        if (!coll.isHit() || (!coll.is_internal && coll.dist > res.x - Const::EPS)) continue;
        double t, u, v, len = 2 * Const::PI / SUBSURFACE_NUM, f;

        for (int j = 0; j < SUBSURFACE_NUM; j++)
        {
            t = coll.dist, u = 0.5, v = j * len + Const::randDouble();
            for (int s = 0; s < ITERATION_NUM; s++)
            {
                Vector3 F = start + d * t - P(i, u, v),
                        dPdu = m_dPdu(i, u, v),
                        dPdv = m_dPdv(i, u, v);
                double D = Vector3::mix(d, dPdu, dPdv);
                if (abs(D) < Const::EPS) break;

                double dt = Vector3::mix(F, dPdu, dPdv) / D,
                       du = Vector3::mix(d, F, dPdv) / D,
                       dv = Vector3::mix(d, dPdu, F) / D;
                f = F.mod2(), t -= dt, u += du, v += dv;
                if (f < 1e-10 && u > -Const::EPS && u < 1 + Const::EPS)
                {
                    if (t > Const::EPS && t < res.x - Const::EPS)
                        res = Vector3(t, u, v), curve_id = i;
                    break;
                }
                if (abs(dt) < Const::EPS && abs(du) < Const::EPS && abs(dv) < Const::EPS) break;
            }
        }
    }
    if (res.x < 1e9)
    {
        Vector2 p = m_curves[curve_id].dP(res.y);
        Vector3 n = Vector3(-p.y * cos(res.z), -p.y * sin(res.z), p.x);
        if (n.dot(d) < Const::EPS)
            return Collision(start, d, res.x, curve_id + res.y, fmod(res.z, 2 * Const::PI), n, this);
        else
            return Collision(start, d, res.x, curve_id + res.y, fmod(res.z, 2 * Const::PI), -n, this);
    }
    else
        return Collision();
}

Color RotationBody::getTextureColor(const Collision& coll) const
{
    if (m_material->hasTexture())
    {
        double u = fmod(coll.v - m_arg + 4 * Const::PI, 2 * Const::PI) / 2 / Const::PI,
               v = 1 - coll.u / m_curves.size();
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
    Json::Value curves;
    for (auto c : m_curves) curves.append(c.toJson());
    object["type"] = "RotationBody";
    object["o"] = m_o.toJson();
    object["curves"] = curves;
    if (m_material->hasTexture()) object["texture_arg"] = m_arg * 180 / Const::PI;
    return object;
}

void RotationBody::m_init()
{
    for (auto c : m_curves)
    {
        double r = std::max(abs(c.L), abs(c.R));
        m_r = std::max(m_r, r), m_h = std::max(m_h, c.U);
        m_sub_cylinders.push_back(new Cylinder(Vector3(m_o.x, m_o.y, m_o.z + c.D), r, c.U - c.D));
    }
    m_bounding_cylinder = new Cylinder(m_o, m_r, m_h);
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
