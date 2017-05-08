#include "object/object.h"

Json::Value Object::toJson() const
{
    Json::Value object;
    object["material"] = m_material->toJson();
    return object;
}
