#include "common/const.h"
#include "object/plane.h"

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
