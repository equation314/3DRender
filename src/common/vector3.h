#ifndef VECTOR3_H
#define VECTOR3_H

#include "common/const.h"
#include "common/vector2.h"

#include <ostream>
#include <json/value.h>

struct Vector3
{
    Vector3()
        : x(0), y(0), z(0) {}
    Vector3(double x, double y, double z)
        : x(x), y(y), z(z) {}
    Vector3(const Json::Value& vec);

    Vector3 operator-() const { return Vector3(-x, -y, -z); }

    Vector3 operator*(double k) const { return Vector3(x * k, y * k, z * k); }

    Vector3 operator/(double k) const { return Vector3(x / k, y / k, z / k); }

    // 坐标分量
    double& operator[](int i) { return !i ? x : i == 1 ? y : z; }
    const double operator[](int i) const { return !i ? x : i == 1 ? y : z; }

    // 混合积
    static double mix(const Vector3& A, const Vector3& B, const Vector3& C);

    // 点积
    double dot(const Vector3& B) const { return x * B.x + y * B.y + z * B.z; }

    // 模长
    double mod() const { return sqrt(x * x + y * y + z * z); }

    // 模长平方
    double mod2() const { return x * x + y * y + z * z; }

    // 转为二维向量，忽略第 3 维
    Vector2 toVector2() const { return Vector2(x, y); }

    // 单位化
    Vector3 unitize() const
    {
        double m = mod();
        return *(this) / (m < Const::EPS ? 1 : m);
    }

    // 任意一个与自己垂直的向量
    Vector3 getAnVerticalVector() const;

    // 反射
    Vector3 reflect(const Vector3& n) const;

    // 折射
    Vector3 refract(const Vector3& n, double rindex) const;

    // 散射
    Vector3 diffuse() const;

    // 随机单位向量
    static Vector3 randVector();

    Json::Value toJson() const;

    double x, y, z;
};

inline Vector3 operator+(const Vector3& A, const Vector3& B)
{
    return Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
}

inline Vector3 operator-(const Vector3& A, const Vector3& B)
{
    return Vector3(A.x - B.x, A.y - B.y, A.z - B.z);
}

inline Vector3 operator*(const Vector3& A, const Vector3& B)
{
    return Vector3(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
}

inline double Vector3::mix(const Vector3& A, const Vector3& B, const Vector3& C)
{
    return A.dot(B * C);
}

inline std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
    return out << '(' << v.x << ' ' << v.y << ' ' << v.z << ')';
}

inline Vector3 Vector3::getAnVerticalVector() const
{
    Vector3 v = (*this) * (Vector3(0, 0, 1));
    if (v.mod2() < Const::EPS) v = (*this) * (Vector3(1, 0, 0));
    return v.unitize();
}

inline Vector3 Vector3::reflect(const Vector3& n) const
{
    return *this - n * (2 * this->dot(n));
}

inline Vector3 Vector3::refract(const Vector3& n, double rindex) const
{
    double ni = 1 / rindex; // 折射率的倒数，nT/nI
    double cosI = this->dot(n), cosT2 = 1 - ni * ni * (1 - cosI * cosI);
    if (cosT2 >= 0)
        return (*this) * ni - n * (sqrt(cosT2) + cosI * ni);
    else
        return Vector3();
}

inline Vector3 Vector3::diffuse() const
{
    double theta = acos(sqrt(Const::randDouble())), phi = 2 * Const::PI * Const::randDouble();
    Vector3 dir(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));
    Vector3 dx = getAnVerticalVector(), dy = (*this) * (dx);
    return dx * dir.x + dy * dir.y + operator*(dir.z);
}

inline Vector3 Vector3::randVector()
{
    Vector3 v;
    for (;;)
    {
        v.x = 2 * Const::randDouble() - 1;
        v.y = 2 * Const::randDouble() - 1;
        v.z = 2 * Const::randDouble() - 1;
        if (v.mod2() > Const::EPS && v.mod2() < 1 + Const::EPS) break;
    }
    return v.unitize();
}

#endif // VECTOR3_H
