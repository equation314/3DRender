#include "common/const.h"
#include "object/plane.h"

Collision Plane::collide(const Vector3& start, const Vector3& dir) const
{
    double n = m_n.dot(start) + m_d, d = m_n.dot(dir);
    if (abs(d) < Const::EPS) return Collision();
    double t = -n / d;
    if (t < Const::EPS) return Collision();
    return Collision(start, dir, t, n > Const::EPS ? m_n : -m_n, this);
}
