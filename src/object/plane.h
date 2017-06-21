#ifndef PLANE_H
#define PLANE_H

#include "common/vector3.h"
#include "object/object.h"

class Plane : public Object
{
public:
    Plane(const Vector3& n, double d, const Material* m = nullptr);
    Plane(const Json::Value& object);

    virtual std::string getType() const override { return "Plane"; }

    // 与视线相交
    virtual Collision collide(const Ray& ray) const override;

    // 交点处的纹理颜色
    virtual Color getTextureColor(const Collision& coll) const override;

    virtual Json::Value toJson() const override;

    // 设置纹理坐标轴
    void setTextureAxis(const Vector3& o, const Vector3& dx, const Vector3& dy) { m_o = o, m_dx = dx, m_dy = dy; }

private:
    Vector3 m_n;             // 法向量
    double m_d;              // 直线方程为 n * P + d = 0
    Vector3 m_o, m_dx, m_dy; // 纹理原点和坐标轴向量
};

#endif // PLANE_H
