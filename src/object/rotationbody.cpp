#include "common/const.h"
#include "object/rotationbody.h"

RotationBody::RotationBody(const Material* m, const Vector3& o, const Curves& curves)
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
    return Collision();
}

Color RotationBody::getTextureColor(const Vector3& p) const
{
    return Color(1, 1, 1);
}

Vector3 RotationBody::P(double u, double v) const
{
    int i = u - Const::EPS;
    Vector2 p = m_curves[i].P(u - i);
    return Vector3(p.x * cos(v), p.x * sin(v), p.y) + m_o;
}

void RotationBody::saveOBJ(const std::string& file, int density) const
{
    std::vector<Vector3> points;
    std::vector<std::vector<int>> meshes;
    double du = 1.0 / density, dv = 2 * Const::PI / density;
    int n = 0;
    for (unsigned int t = 0; t < m_curves.size(); t++)
        for (int i = 0; i <= density; i++)
            for (int j = 0; j <= density; j++)
            {
                double u = i * du, v = j * dv;
                n++;
                points.push_back(P(t + u, v));
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
