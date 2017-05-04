#include "common/color.h"

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

Color Color::confine() const
{
    return Color(std::min(r, 1.0), std::min(g, 1.0), std::min(b, 1.0));
}
