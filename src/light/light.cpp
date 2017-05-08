#include "light/light.h"

Json::Value Light::toJson() const
{
    Json::Value light;
    light["color"] = m_color.toJson();
    return light;
}
