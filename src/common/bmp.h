#ifndef BMP_H
#define BMP_H

#pragma pack(1)

#include "common/color.h"

#include <string>

class Bmp
{
public:
    Bmp(int w, int h, const Color& background = Color());
    Bmp(const std::string& file);
    ~Bmp();

    int getW() const { return m_w; }
    int getH() const { return m_h; }
    Color getColor(int x, int y) const;
    Color getColor(double u, double v) const;
    void setColor(int x, int y, const Color& color);
    void save(const std::string& file) const;

private:
    typedef unsigned char byte;
    typedef unsigned short word;
    typedef unsigned int dword;

    struct BITMAPFILEHEADER
    {
        word bfType;
        dword bfSize;
        word bfReserved1;
        word bfReserved2;
        dword bfOffBits;
    };
    struct BITMAPINFOHEADER
    {
        dword biSize;
        dword biWidth;
        dword biHeight;
        word biPlanes;
        word biBitCount;
        dword biCompression;
        dword biSizeImage;
        dword biXPelsPerMeter;
        dword biYPelsPerMeter;
        dword biClrUsed;
        dword biClrImportant;
    };
    struct BmpColor
    {
        byte r, g, b;
        BmpColor() {}
        BmpColor(byte r, byte g, byte b)
            : r(r), g(g), b(b) {}
    };

    int m_w, m_h;
    BITMAPFILEHEADER m_bf;
    BITMAPINFOHEADER m_bi;
    BmpColor** m_data;
};

#endif // BMP_H
