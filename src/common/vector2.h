#ifndef VECTOR2_H
#define VECTOR2_H

#include "common/const.h"

#include <ostream>
#include <json/value.h>

struct Vector2
{
    Vector2()
        : x(0), y(0) {}
    Vector2(double x, double y)
        : x(x), y(y) {}
    Vector2(const Json::Value& vec);

    Vector2 operator-() const { return Vector2(-x, -y); }

    Vector2 operator*(double k) const { return Vector2(x * k, y * k); }

    Vector2 operator/(double k) const { return Vector2(x / k, y / k); }

    // 点积
    double dot(const Vector2& B) const { return x * B.x + y * B.y; }

    // 模长
    double mod() const { return sqrt(x * x + y * y); }

    // 模长平方
    double mod2() const { return x * x + y * y; }

    // 极角
    double arg() const { return atan2(y, x); }

    // 单位化
    Vector2 unitize() const
    {
        double m = mod();
        return *(this) / (m < Const::EPS ? 1 : m);
    }

    Json::Value toJson() const;

    double x, y;
};

inline Vector2 operator+(const Vector2& A, const Vector2& B)
{
    return Vector2(A.x + B.x, A.y + B.y);
}

inline Vector2 operator-(const Vector2& A, const Vector2& B)
{
    return Vector2(A.x - B.x, A.y - B.y);
}

inline double operator*(const Vector2& A, const Vector2& B)
{
    return A.x * B.y - A.y * B.x;
}

inline std::ostream& operator<<(std::ostream& out, const Vector2& v)
{
    return out << '(' << v.x << ' ' << v.y << ')';
}

#endif // VECTOR2_H
