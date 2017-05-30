#ifndef VECTOR2_H
#define VECTOR2_H

#include <ostream>
#include <json/value.h>

struct Vector2
{
public:
    Vector2()
        : x(0), y(0) {}
    Vector2(double x, double y)
        : x(x), y(y) {}
    Vector2(const Json::Value& vec);

    friend Vector2 operator+(const Vector2& A, const Vector2& B);
    friend Vector2 operator-(const Vector2& A, const Vector2& B);
    friend double operator*(const Vector2& A, const Vector2& B); // 叉积
    friend std::ostream& operator<<(std::ostream& out, const Vector2& v);
    Vector2 operator-() const;
    Vector2 operator*(double k) const;
    Vector2 operator/(double k) const;
    double dot(const Vector2& B) const; // 点积
    double mod() const;                 // 模长
    double mod2() const;                // 模长平方
    double arg() const;                 // 极角

    Vector2 unitize() const; // 单位化

    Json::Value toJson() const;

    double x, y;
};

#endif // VECTOR2_H
