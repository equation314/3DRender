#ifndef VECTOR3_H
#define VECTOR3_H

#include <ostream>
#include <json/value.h>

struct Vector3
{
public:
    Vector3()
        : x(0), y(0), z(0) {}
    Vector3(double x, double y, double z)
        : x(x), y(y), z(z) {}
    Vector3(const Json::Value& vec);

    friend Vector3 operator+(const Vector3& A, const Vector3& B);
    friend Vector3 operator-(const Vector3& A, const Vector3& B);
    friend Vector3 operator*(const Vector3& A, const Vector3& B); // 叉积
    friend std::ostream& operator<<(std::ostream& out, const Vector3& v);
    static double mix(const Vector3& A, const Vector3& B, const Vector3& C); // 混合积
    Vector3 operator-() const;
    Vector3 operator*(double k) const;
    Vector3 operator/(double k) const;
    double dot(const Vector3& B) const; // 点积
    double mod() const;                 // 模长
    double mod2() const;                // 模长平方

    Vector3 unitize() const;                                // 单位化
    Vector3 reflect(const Vector3& n) const;                // 反射
    Vector3 refract(const Vector3& n, double rindex) const; // 折射

    Json::Value toJson() const;

    double x, y, z;
};

#endif // VECTOR3_H
