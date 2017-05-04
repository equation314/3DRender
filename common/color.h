#ifndef COLOR_H
#define COLOR_H

#include <ostream>

struct Color
{
public:
    Color()
        : r(0), g(0), b(0) {}
    Color(double r, double g, double b)
        : r(r), g(g), b(b) {}

    friend Color operator+(const Color& A, const Color& B);
    friend Color operator-(const Color& A, const Color& B);
    friend Color operator*(const Color& A, const Color& B);
    friend std::ostream& operator<<(std::ostream& out, const Color& c);
    Color& operator+=(const Color& B);
    Color& operator-=(const Color& B);
    Color& operator*=(const Color& B);
    Color operator*(double k) const;
    Color operator/(double k) const;
    Color confine() const;

    double r, g, b;
};

#endif // COLOR_H
