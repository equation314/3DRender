#include "common/vector3.h"

#include <cstdio>

Vector3::Vector3(const Json::Value& vec)
    : x(0), y(0), z(0)
{
    if (vec.isString())
    {
        std::string s = vec.asString();
        sscanf(s.c_str(), "(%lf,%lf,%lf)", &x, &y, &z);
    }
}

Json::Value Vector3::toJson() const
{
    char str[256];
    sprintf(str, "(%lf, %lf, %lf)", x, y, z);
    return Json::Value(str);
}
