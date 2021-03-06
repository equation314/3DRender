#ifndef RECTLIGHT_H
#define RECTLIGHT_H

#include "common/vector3.h"
#include "light/light.h"

class RectLight : public Light
{
public:
    RectLight(const Color& c, const Vector3& o, const Vector3& n, const Vector3& dx, const Vector3& dy, double power = 1)
        : Light(c, power), m_o(o), m_n(n), m_dx(dx), m_dy(dy) {}
    RectLight(const Json::Value& light)
        : Light(light), m_o(light["o"]), m_n(light["n"]), m_dx(light["dx"]), m_dy(light["dy"]) {}

    // 发光点
    virtual Vector3 getSource() const override { return m_o; }

    // 与视线相交
    virtual Collision collide(const Ray& ray) const override;

    // 计算阴影比例
    virtual double getShadowRatio(const Scene* scene, const Vector3& p) const override;

    // 发射光子
    virtual Photon emitPhoton(double power) const override;

    virtual Json::Value toJson() const override;

private:
    Vector3 m_o, m_n;   // 矩形中心点、光源朝向
    Vector3 m_dx, m_dy; // 横向、纵向向量
};

#endif // RECTLIGHT_H
