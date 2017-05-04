#ifndef BMP_H
#define BMP_H

#include <string>

#include "common/color.h"

class Bmp
{
public:
    Bmp(int w, int h);
    ~Bmp();

    void setColor(int x, int y, const Color& color);
    void save(const std::string& file) const;

private:
    struct BmpColor
    {
        int r, g, b;
        BmpColor() {}
        BmpColor(int r, int g, int b)
            : r(r), g(g), b(b) {}
    };
    int m_w, m_h;
    BmpColor** m_data;

    void m_writeInt(int x, int bytes, FILE* f) const;
    void m_writeHead(FILE* f) const;
    void m_writeData(FILE* f) const;
};

#endif // BMP_H
