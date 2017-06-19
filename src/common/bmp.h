#ifndef BMP_H
#define BMP_H

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

    // 直接获取像素点颜色
    Color getColor(int x, int y) const
    {
        return Color(m_data[x][y].r, m_data[x][y].g, m_data[x][y].b) / 255;
    }

    // 根据像素位置计算颜色
    Color getColor(double u, double v) const;

    void setColor(int x, int y, const Color& color)
    {
        m_data[x][y] = BmpColor(color.r * 255 + 0.5, color.g * 255 + 0.5, color.b * 255 + 0.5);
    }

    void save(const std::string& file) const;
    std::string getFilename() const { return m_file_name; }

private:
    typedef unsigned char byte;
    typedef unsigned short word;
    typedef unsigned int dword;

#pragma pack(push, 1)
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
#pragma pack(pop)
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
    std::string m_file_name;
};

#endif // BMP_H
