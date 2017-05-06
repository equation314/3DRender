#include "common/const.h"
#include "object/sphere.h"

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
