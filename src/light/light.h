#ifndef LIGHT_H
#define LIGHT_H

#include "common/color.h"
#include "common/photon.h"
#include "object/collision.h"

class Scene;
class PointLight;

class Light
{
public:
    Light(const Color& c, double power = 1)
        : m_color(c), m_power(power) {}
    Light(const Json::Value& light)
        : m_color(light["color"]), m_power(light["power"].asDouble()) {}
    virtual ~Light() {}

    Color getColor() const { return m_color; }
    double getPower() const { return m_power; }

    // 发光点
    virtual Vector3 getSource() const = 0;

    // 与视线相交
    virtual Collision collide(const Vector3& start, const Vector3& dir) const = 0;

    // 计算阴影比例，值越小阴影越深
    virtual double getShadowRatio(const Scene* scene, const Vector3& p) const = 0;

    // 发射光子
    virtual Photon emitPhoton(double power) const = 0;

    // 保存为 JSON 格式
    virtual Json::Value toJson() const;

    // 从 JSON 导入物体
    static Light* loadFromJson(const Json::Value& value);

protected:
    Color m_color;  // 光色
    double m_power; // 光能
};

#endif // LIGHT_H
