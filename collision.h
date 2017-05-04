#ifndef COLLISION_H
#define COLLISION_H

#include "common/vector3.h"

class Object;

struct Collision
{
    Collision();
    Collision(const Vector3& start, const Vector3& dir, double t, const Vector3& n, const Object* obj);

    const Object* object;             // 相交的物体
    Vector3 ray_start, ray_dir, o, n; // 射线起点，射线方向，交点，交点处物体的法向量，自动单位化
    double dist;                      // 交点对应的射线参数，ray_d 单位化后就是交点与射线起点间的距离

    // 是否有交点
    bool isHit() const { return dist < 1e9; }
};

#endif // COLLISION_H
