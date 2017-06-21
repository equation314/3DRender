#ifndef COLLISION_H
#define COLLISION_H

#include "common/ray.h"

class Light;
class Object;

struct Collision
{
    // 不相交
    Collision();

    // 与物体相交
    Collision(const Ray& ray, double t, const Vector3& n, const Object* obj, bool in = false);

    // 与物体相交，包含参数
    Collision(const Ray& ray, double t, double u, double v, const Vector3& n, const Object* obj, bool in = false);

    // 与光源相交
    Collision(const Ray& ray, double t, const Light* l);

    const Object* object; // 相交的物体
    const Light* light;   // 相交的光源
    Ray ray;              // 射线
    Vector3 p, n;         // 交点，交点处物体的法向量(只对与物体相交有效)，自动单位化
    double dist;          // 交点对应的射线参数，ray_d 单位化后就是交点与射线起点间的距离
    double u, v;          // 如果是参数曲面，保存交点处的参数
    bool is_internal;     // 射线起点是否在物体内部（可选）

    // 是否有交点
    bool isHit() const { return dist < 1e9; }

    // 是否是与物体相交
    bool atObject() const { return object != nullptr; }

    // 是否是与光源相交
    bool atLight() const { return light != nullptr; }
};

#endif // COLLISION_H
