#ifndef OBJECT_H
#define OBJECT_H

#include "object/collision.h"
#include "object/material.h"

class Plane;
class Sphere;
class Cylinder;
class RotationBody;

class Object
{
public:
    virtual ~Object();

    uint64 getIdentifier() const { return m_identifier; }
    const Material* getMaterial() const { return m_material; }

    virtual std::string getType() const = 0;

    // 与视线相交
    virtual Collision collide(const Ray& ray) const = 0;

    // 交点处的纹理颜色
    virtual Color getTextureColor(const Collision& coll) const = 0;

    // 保存为 JSON 格式
    virtual Json::Value toJson() const;

    // 保存 JSON 到文件
    virtual void save(const std::string& file) const;

    // 从 JSON 导入物体
    static Object* loadFromJson(const Json::Value& value);

    // 从文件导入物体
    static Object* loadFrom(const std::string& file);

protected:
    Object(const Material* m);
    Object(const Json::Value& object);

    const Material* m_material;
    uint64 m_identifier; // 标识符

private:
    bool m_can_delete_material;
};

#endif // OBJECT_H
