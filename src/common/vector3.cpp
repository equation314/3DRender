#include "common/const.h"
#include "common/vector3.h"

Vector3::Vector3(const Json::Value& vec)
    : x(0), y(0), z(0)
{
    if (vec.isString())
    {
        string s = vec.asString();
        sscanf(s.c_str(), "(%lf,%lf,%lf)", &x, &y, &z);
    }
}

Vector3 operator+(const Vector3& A, const Vector3& B)
{
    return Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
}

Vector3 operator-(const Vector3& A, const Vector3& B)
{
    return Vector3(A.x - B.x, A.y - B.y, A.z - B.z);
}

Vector3 operator*(const Vector3& A, const Vector3& B)
{
    return Vector3(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
}

double Vector3::mix(const Vector3& A, const Vector3& B, const Vector3& C)
{
    return A.dot(B * C);
}

std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
    return out << '(' << v.x << ' ' << v.y << ' ' << v.z << ')';
}

Vector3 Vector3::operator-() const
{
    return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator*(double k) const
{
    return Vector3(x * k, y * k, z * k);
}

Vector3 Vector3::operator/(double k) const
{
    return Vector3(x / k, y / k, z / k);
}

double Vector3::dot(const Vector3& B) const
{
    return x * B.x + y * B.y + z * B.z;
}

double Vector3::mod() const
{
    return sqrt(x * x + y * y + z * z);
}

double Vector3::mod2() const
{
    return x * x + y * y + z * z;
}

double& Vector3::operator[](int id)
{
    return !id ? x : id == 1 ? y : z;
}

const double Vector3::operator[](int id) const
{
    return !id ? x : id == 1 ? y : z;
}

Vector2 Vector3::toVector2() const
{
    return Vector2(x, y);
}

Vector3 Vector3::unitize() const
{
    double m = mod();
    return *(this) / (m < Const::EPS ? 1 : m);
}

Vector3 Vector3::diffuse() const
{
    double theta = acos(sqrt(Const::randDouble())), phi = 2 * Const::PI * Const::randDouble();
    Vector3 dir(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));
    Vector3 dx = getAnVerticalVector(), dy = (*this) * (dx);
    return dx * dir.x + dy * dir.y + operator*(dir.z);
}

Vector3 Vector3::reflect(const Vector3& n) const
{
    return *this - n * (2 * this->dot(n));
}

Vector3 Vector3::refract(const Vector3& n, double rindex) const
{
    double ni = 1 / rindex; // 折射率的倒数，nT/nI
    double cosI = this->dot(n), cosT2 = 1 - ni * ni * (1 - cosI * cosI);
    if (cosT2 >= 0)
        return (*this) * ni - n * (sqrt(cosT2) + cosI * ni);
    else
        return Vector3();
}

Vector3 Vector3::getAnVerticalVector() const
{
    Vector3 v = (*this) * (Vector3(0, 0, 1));
    if (v.mod2() < Const::EPS) v = (*this) * (Vector3(1, 0, 0));
    return v.unitize();
}

Vector3 Vector3::randVector()
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

Json::Value Vector3::toJson() const
{
    char str[256];
    sprintf(str, "(%lf, %lf, %lf)", x, y, z);
    return Json::Value(str);
}
