#include "common/const.h"
#include "object/plane.h"

Collision Plane::collide(const Vector3& start, const Vector3& dir) const
{
    double n = m_n.dot(start) + m_d, d = m_n.dot(dir);
    if (abs(n) < EPS || abs(d) < EPS) return Collision();
    double t = - n / d;
    if (t < 0) return Collision();
    return Collision(start, dir, t, n > EPS ? m_n : m_n * -1, this);
}
