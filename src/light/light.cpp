#include "light/light.h"
#include "light/pointlight.h"
#include "light/rectlight.h"

Json::Value Light::toJson() const
{
    Json::Value light;
    light["color"] = m_color.toJson();
    light["power"] = m_power;
    return light;
}

Light* Light::loadFromJson(const Json::Value& value)
{
    std::string type = value["type"].asString();
    if (type == "PointLight")
        return new PointLight(value);
    else if (type == "RectLight")
        return new RectLight(value);
    else
        return nullptr;
}
