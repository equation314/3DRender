#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "common/vector3.h"
#include "light/light.h"

class PointLight : public Light
{
public:
    PointLight(const Color& c, const Vector3& o)
        : Light(c), m_o(o) {}
    PointLight(const Json::Value& light)
        : Light(light), m_o(light["o"]) {}

    // 发光点
    virtual Vector3 getSource() const override { return m_o; }

    // 与视线相交
    virtual Collision collide(const Vector3& start, const Vector3& dir) const override;

    // 计算阴影比例
    virtual double getShadowRatio(const Scene* scene, const Vector3& p) const override;

    virtual Json::Value toJson() const override;

private:
    Vector3 m_o;
};

#endif // POINTLIGHT_H
