#include "common/color.h"
#include "common/const.h"

Color::Color(const Json::Value& color)
    : r(0), g(0), b(0)
{
    if (color.isString())
    {
        string s = color.asString();
        sscanf(s.c_str(), "(%lf,%lf,%lf)", &r, &g, &b);
    }
}

Color operator+(const Color& A, const Color& B)
{
    return Color(A.r + B.r, A.g + B.g, A.b + B.b);
}

Color operator-(const Color& A, const Color& B)
{
    return Color(A.r - B.r, A.g - B.g, A.b - B.b);
}

Color operator*(const Color& A, const Color& B)
{
    return Color(A.r * B.r, A.g * B.g, A.b * B.b);
}

std::ostream& operator<<(std::ostream& out, const Color& c)
{
    return out << '(' << c.r << ' ' << c.g << ' ' << c.b << ')';
}

Color Color::operator*(double k) const
{
    return Color(r * k, g * k, b * k);
}

Color Color::operator/(double k) const
{
    return Color(r / k, g / k, b / k);
}

Color& Color::operator+=(const Color& B)
{
    return *this = *this + B;
}

Color& Color::operator-=(const Color& B)
{
    return *this = *this - B;
}

Color& Color::operator*=(const Color& B)
{
    return *this = *this * B;
}

Color Color::inverse() const
{
    return Color(1 - r, 1 - g, 1 - b);
}

Color Color::exp() const
{
    return Color(std::exp(r), std::exp(g), std::exp(b));
}

Color Color::confine() const
{
    return Color(std::max(std::min(r, 1.0), 0.0), std::max(std::min(g, 1.0), 0.0), std::max(std::min(b, 1.0), 0.0));
}

Json::Value Color::toJson() const
{
    char str[256];
    sprintf(str, "(%lf, %lf, %lf)", r, g, b);
    return Json::Value(str);
}
