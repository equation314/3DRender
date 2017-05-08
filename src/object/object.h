#ifndef OBJECT_H
#define OBJECT_H

#include "collision.h"
#include "object/material.h"

class Plane;
class Sphere;

class Object
{
public:
    Object(const Material* m)
        : m_material(m) {}
    Object(const Json::Value& object)
        : m_material(new Material(object["material"])) {}
    virtual ~Object() {}

    const Material* getMaterial() const { return m_material; }

    // 与视线相交
    virtual Collision collide(const Vector3& start, const Vector3& dir) const = 0;

    // 某点的纹理颜色
    virtual Color getTextureColor(const Vector3& p) const = 0;

    virtual Json::Value toJson() const;

protected:
    const Material* m_material;
};

#endif // OBJECT_H
