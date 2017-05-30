#include "common/const.h"
#include "common/vector2.h"

Vector2::Vector2(const Json::Value& vec)
    : x(0), y(0)
{
    if (vec.isString())
    {
        string s = vec.asString();
        sscanf(s.c_str(), "(%lf,%lf)", &x, &y);
    }
}

Vector2 operator+(const Vector2& A, const Vector2& B)
{
    return Vector2(A.x + B.x, A.y + B.y);
}

Vector2 operator-(const Vector2& A, const Vector2& B)
{
    return Vector2(A.x - B.x, A.y - B.y);
}

double operator*(const Vector2& A, const Vector2& B)
{
    return A.x * B.y - A.y * B.x;
}

std::ostream& operator<<(std::ostream& out, const Vector2& v)
{
    return out << '(' << v.x << ' ' << v.y << ')';
}

Vector2 Vector2::operator-() const
{
    return Vector2(-x, -y);
}

Vector2 Vector2::operator*(double k) const
{
    return Vector2(x * k, y * k);
}

Vector2 Vector2::operator/(double k) const
{
    return Vector2(x / k, y / k);
}

double Vector2::dot(const Vector2& B) const
{
    return x * B.x + y * B.y;
}

double Vector2::mod() const
{
    return sqrt(x * x + y * y);
}

double Vector2::mod2() const
{
    return x * x + y * y;
}

double Vector2::arg() const
{
    return atan2(y, x);
}

Vector2 Vector2::unitize() const
{
    double m = mod();
    return *(this) / (m < Const::EPS ? 1 : m);
}

Json::Value Vector2::toJson() const
{
    char str[256];
    sprintf(str, "(%lf, %lf)", x, y);
    return Json::Value(str);
}
