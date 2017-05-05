#ifndef VECTOR3_H
#define VECTOR3_H

#include <ostream>

struct Vector3
{
public:
    Vector3()
        : x(0), y(0), z(0) {}
    Vector3(double x, double y, double z)
        : x(x), y(y), z(z) {}

    friend Vector3 operator+(const Vector3& A, const Vector3& B);
    friend Vector3 operator-(const Vector3& A, const Vector3& B);
    friend Vector3 operator*(const Vector3& A, const Vector3& B);
    friend std::ostream& operator<<(std::ostream& out, const Vector3& v);
    Vector3 operator-() const;
    Vector3 operator*(double k) const;
    Vector3 operator/(double k) const;
    double dot(const Vector3& B) const;
    double mod() const;
    double mod2() const;

    // 单位化
    Vector3 unitize() const;

    // 反射
    Vector3 reflect(const Vector3& n) const;

    // 折射
    Vector3 refract(const Vector3& n, double rindex) const;

    double x, y, z;
};

#endif // VECTOR3_H
