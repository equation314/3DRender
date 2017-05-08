#include "common/const.h"
#include "object/plane.h"

Plane::Plane(const Material* m, const Vector3& n, double d)
    : Object(m), m_n(n.unitize()), m_d(d),
      m_o(0, 0, 0)
{
    m_dx = m_n * Vector3(0, 0, -1);
    if (m_dx.mod2() < Const::EPS)
        m_dx = Vector3(1, 0, 0);
    else
        m_dx = m_dx.unitize();
    m_dy = (m_dx * m_n).unitize();
}

Collision Plane::collide(const Vector3& start, const Vector3& dir) const
{
    double n = m_n.dot(start) + m_d, d = m_n.dot(dir);
    if (abs(d) < Const::EPS) return Collision();
    double t = -n / d;
    if (t < Const::EPS) return Collision();
    if (n > Const::EPS)
        return Collision(start, dir, t, m_n, false, this);
    else
        return Collision(start, dir, t, -m_n, true, this);
}

Color Plane::getTextureColor(const Vector3& p) const
{
    if (m_material->hasTexture())
    {
        double u = (p - m_o).dot(m_dx) / m_dx.mod2(),
               v = (p - m_o).dot(m_dy) / m_dy.mod2();
        u -= floor(u), v -= floor(v);
        return m_material->getTextureColor(u, v);
    }
    else
        return Color(1, 1, 1);
}
