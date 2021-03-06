#include "common/vector2.h"

#include <cstdio>

Vector2::Vector2(const Json::Value& vec)
    : x(0), y(0)
{
    if (vec.isString())
    {
        std::string s = vec.asString();
        sscanf(s.c_str(), "(%lf,%lf)", &x, &y);
    }
}

Json::Value Vector2::toJson() const
{
    char str[256];
    sprintf(str, "(%lf, %lf)", x, y);
    return Json::Value(str);
}
