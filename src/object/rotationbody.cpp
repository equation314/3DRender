#include "common/const.h"
#include "object/rotationbody.h"

RotationBody::RotationBody(const Vector3& o, const Curves& curves, const Material* m)
    : Object(m), m_o(o), m_curves(curves)
{
}

RotationBody::RotationBody(const Json::Value& object)
    : Object(object), m_o(object["o"]), m_curves()
{
    for (auto c : object["curves"])
        m_curves.push_back(BezierCurve3(c));
}

Collision RotationBody::collide(const Vector3& start, const Vector3& dir) const
{
    const int NUM = 8;
    Vector3 d = dir.unitize();
    bool in = false;
    int curve_id = 0;
    Vector3 res(1e9, 0, 0);
    for (size_t i = 0; i < m_curves.size(); i++)
    {
        double len = Const::PI * 2 / NUM;
        for (int j = 0; j < NUM; j++)
        {
            double l = j * len, r = j * len + len, t = 0, u = 0.5, v = l + Const::randDouble() * len, f;
            for (int s = 0; s < 10; s++)
            {
                Vector3 F = start + d * t - P(i, u, v),
                        dPdu = m_dPdu(i, u, v),
                        dPdv = m_dPdv(i, u, v);
                double D = Vector3::mix(d, dPdu, dPdv);
                if (abs(D) < Const::EPS) break;

                double dt = Vector3::mix(F, dPdu, dPdv) / D,
                       du = Vector3::mix(d, F, dPdv) / D,
                       dv = Vector3::mix(d, dPdu, F) / D;
                f = F.mod2();
                t -= dt, u += du, v += dv;
                if (f < 1e-10 && u > -Const::EPS && u < 1 + Const::EPS)
                {
                    if (t < Const::EPS)
                        in = true;
                    else if (t < res.x - Const::EPS)
                        res = Vector3(t, u, v), curve_id = i;
                    break;
                }
            }
        }
    }
    if (res.x < 1e9)
    {
        Vector2 p = m_curves[curve_id].dP(res.y);
        Vector3 n = Vector3(-p.y * cos(res.z), -p.y * sin(res.z), p.x);
        if (n.dot(d) < Const::EPS)
            return Collision(start, d, res.x, n, in, this);
        else
            return Collision(start, d, res.x, -n, in, this);
    }
    else
        return Collision();
}

Color RotationBody::getTextureColor(const Vector3& p) const
{
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
    return object;
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
