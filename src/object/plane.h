#ifndef PLANE_H
#define PLANE_H

#include "common/vector3.h"
#include "object/object.h"

class Plane : public Object
{
public:
    Plane(const Material* m, const Vector3& n, double d);
    Plane(const Json::Value& object);

    // 与视线相交
    virtual Collision collide(const Vector3& start, const Vector3& dir) const override;

    // 某点的纹理颜色
    virtual Color getTextureColor(const Vector3& p) const override;

    // 设置纹理坐标轴
    void setTextureAxis(const Vector3& o, const Vector3& dx, const Vector3& dy) { m_o = o, m_dx = dx, m_dy = dy; }

    virtual Json::Value toJson() const override;

private:
    Vector3 m_n;             // 法向量
    double m_d;              // 直线方程为 n * P + d = 0
    Vector3 m_o, m_dx, m_dy; // 纹理原点和坐标轴向量
};

#endif // PLANE_H
