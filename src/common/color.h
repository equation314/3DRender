#ifndef COLOR_H
#define COLOR_H

#include "common/const.h"

#include <ostream>
#include <json/value.h>

struct Color
{
    Color()
        : r(0), g(0), b(0) {}
    Color(double r, double g, double b)
        : r(r), g(g), b(b) {}
    Color(const Json::Value& color);

    Color operator*(double k) const
    {
        return Color(r * k, g * k, b * k);
    }

    Color operator/(double k) const { return Color(r / k, g / k, b / k); }

    Color& operator+=(const Color& B)
    {
        r += B.r, g += B.g, b += B.b;
        return *this;
    }

    Color& operator-=(const Color& B)
    {
        r -= B.r, g -= B.g, b -= B.b;
        return *this;
    }

    Color& operator*=(const Color& B)
    {
        r *= B.r, g *= B.g, b *= B.b;
        return *this;
    }

    Color& operator*=(double k)
    {
        r *= k, g *= k, b *= k;
        return *this;
    }

    Color& operator/=(double k)
    {
        r /= k, g /= k, b /= k;
        return *this;
    }

    // 颜色距离
    double mod2() const { return r * r + g * g + b * b; }

    // 反色
    Color inverse() const { return Color(1 - r, 1 - g, 1 - b); }

    // 每个分量求指数
    Color exp() const
    {
        return Color(std::exp(r), std::exp(g), std::exp(b));
    }

    // 限制颜色取值为 [0, 1]
    Color confine() const
    {
        return Color(std::max(std::min(r, 1.0), 0.0), std::max(std::min(g, 1.0), 0.0), std::max(std::min(b, 1.0), 0.0));
    }

    // 光子映射时的能量
    double power() const { return (r + g + b) / 3; }

    Json::Value toJson() const;

    double r, g, b;
};

inline Color operator+(const Color& A, const Color& B)
{
    return Color(A.r + B.r, A.g + B.g, A.b + B.b);
}

inline Color operator-(const Color& A, const Color& B)
{
    return Color(A.r - B.r, A.g - B.g, A.b - B.b);
}

inline Color operator*(const Color& A, const Color& B)
{
    return Color(A.r * B.r, A.g * B.g, A.b * B.b);
}

inline std::ostream& operator<<(std::ostream& out, const Color& c)
{
    return out << '(' << c.r << ' ' << c.g << ' ' << c.b << ')';
}

#endif // COLOR_H
