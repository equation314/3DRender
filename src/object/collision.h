#ifndef COLLISION_H
#define COLLISION_H

#include "common/vector3.h"

class Light;
class Object;

struct Collision
{
    // 不相交
    Collision();

    // 与物体相交
    Collision(const Vector3& start, const Vector3& dir, double t, const Vector3& n, bool in, const Object* obj);

    // 与光源相交
    Collision(const Vector3& start, const Vector3& dir, double t, const Light* l);

    const Object* object;             // 相交的物体
    const Light* light;               // 相交的光源
    Vector3 ray_start, ray_dir, p, n; // 射线起点，射线方向，交点，交点处物体的法向量(只对与物体相交有效)，自动单位化
    double dist;                      // 交点对应的射线参数，ray_d 单位化后就是交点与射线起点间的距离
    double is_internal;               // 交点是否在物体内部，即视线是从物体内部向外发出的

    // 是否有交点
    bool isHit() const { return dist < 1e9; }

    // 是否是与物体相交
    bool atObject() const { return object != nullptr; }

    // 是否是与光源相交
    bool atLight() const { return light != nullptr; }
};

#endif // COLLISION_H
