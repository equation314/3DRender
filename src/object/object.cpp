#include "object/object.h"

Object::Object(const Material* m)
    : m_material(m)
{
}

Object::Object(const Json::Value& object)
    : m_material(new Material(object["material"]))
{
}

Json::Value Object::toJson() const
{
    Json::Value object;
    object["material"] = m_material->toJson();
    return object;
}
