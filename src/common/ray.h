#ifndef RAY_H
#define RAY_H

#include "common/vector3.h"

struct Ray
{
    Ray()
        : start(), dir() {}
    Ray(const Vector3& s, const Vector3& d)
        : start(s), dir(d) {}

    // 射线上一点
    Vector3 get(double t) const { return start + dir * t; }

    // 单位化方向
    Ray unitize() const { return Ray(start, dir.unitize()); }

    Vector3 start, dir;
};

#endif // RAY_H
