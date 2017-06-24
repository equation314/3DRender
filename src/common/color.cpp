#include "common/color.h"

#include <cstdio>

Color::Color(const Json::Value& color)
    : r(0), g(0), b(0)
{
    if (color.isString())
    {
        std::string s = color.asString();
        sscanf(s.c_str(), "(%lf,%lf,%lf)", &r, &g, &b);
    }
}

Json::Value Color::toJson() const
{
    char str[256];
    sprintf(str, "(%lf, %lf, %lf)", r, g, b);
    return Json::Value(str);
}
