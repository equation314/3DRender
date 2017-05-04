#ifndef PLANE_H
#define PLANE_H

#include "common/vector3.h"
#include "object/object.h"

class Plane : public Object
{
public:
    Plane(const Vector3& n, double d)
        : Object(), m_n(n.unitize()), m_d(d) {}

    virtual Collision collide(const Vector3& start, const Vector3& dir) const override;

private:
    Vector3 m_n; // 法向量
    double m_d;  // 直线方程为 n * P + d = 0
};

#endif // PLANE_H
