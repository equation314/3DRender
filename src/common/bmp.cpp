#include "common/bmp.h"
#include "common/config.h"
#include "common/const.h"

#include <cstring>
#include <iostream>

Bmp::Bmp(int w, int h, const Color& background)
    : m_w(w), m_h(h),
      m_file_name("")
{
    m_data = new BmpColor*[w];
    for (int i = 0; i < m_w; i++)
    {
        m_data[i] = new BmpColor[h];
        for (int j = 0; j < m_h; j++) setColor(i, j, background);
    }

    memset(&m_bf, 0, sizeof(m_bf));
    memset(&m_bi, 0, sizeof(m_bi));

    m_bf.bfType = 0x4D42;
    m_bf.bfSize = (m_w * 3 + m_w % 4) * m_h + 54;
    m_bf.bfOffBits = 54;

    m_bi.biSize = 40;
    m_bi.biWidth = m_w;
    m_bi.biHeight = m_h;
    m_bi.biPlanes = 1;
    m_bi.biBitCount = 24;
}

Bmp::Bmp(const std::string& file)
    : m_w(0), m_h(0),
      m_file_name(file)
{
    FILE* f = fopen(file.c_str(), "rb");
    if (!f)
    {
        std::cerr << "ERROR: No such BMP file '" + file + "'" << std::endl;
        return;
    }

    fread(&m_bf, 1, sizeof(BITMAPFILEHEADER), f);
    fread(&m_bi, 1, sizeof(BITMAPINFOHEADER), f);

    m_w = m_bi.biWidth, m_h = m_bi.biHeight;
    m_data = new BmpColor*[m_w];
    for (int i = 0; i < m_w; i++) m_data[i] = new BmpColor[m_h];

    for (int j = m_h - 1; j >= 0; j--)
    {
        for (int i = 0; i < m_w; i++)
        {
            fread(&m_data[i][j].b, 1, sizeof(byte), f);
            fread(&m_data[i][j].g, 1, sizeof(byte), f);
            fread(&m_data[i][j].r, 1, sizeof(byte), f);
        }
        for (int i = 0; i < m_w % 4; i++) fgetc(f);
    }

    fclose(f);
}

Bmp::~Bmp()
{
    for (int i = 0; i < m_w; i++) delete[] m_data[i];
    delete[] m_data;
}

Color Bmp::getColor(double u, double v) const
{
    u *= m_w - 1, v *= m_h - 1;
    if (Config::enable_texture_filtering) // 双线性插值
    {
        int u1 = floor(u + Const::EPS), v1 = floor(v + Const::EPS),
            u2 = u1 + 1, v2 = v1 + 1;
        double ru = u2 - u, rv = v2 - v;
        if (u1 < 0) u1 = m_w - 1;
        if (v1 < 0) v1 = m_h - 1;
        if (u2 == m_w) u2 = 0;
        if (v2 == m_h) v2 = 0;
        return getColor(u1, v1) * (ru * rv) + getColor(u1, v2) * (ru * (1 - rv)) +
               getColor(u2, v1) * ((1 - ru) * rv) + getColor(u2, v2) * ((1 - ru) * (1 - rv));
    }
    else
        return getColor(int(u + 0.5), int(v + 0.5));
}

void Bmp::save(const std::string& file) const
{
    FILE* f = fopen(file.c_str(), "wb");

    fwrite(&m_bf, 1, sizeof(BITMAPFILEHEADER), f);
    fwrite(&m_bi, 1, sizeof(BITMAPINFOHEADER), f);
    for (int j = m_h - 1; j >= 0; j--)
    {
        for (int i = 0; i < m_w; i++)
        {
            fwrite(&m_data[i][j].b, 1, sizeof(byte), f);
            fwrite(&m_data[i][j].g, 1, sizeof(byte), f);
            fwrite(&m_data[i][j].r, 1, sizeof(byte), f);
        }
        for (int i = 0; i < m_w % 4; i++) fputc(0, f);
    }

    fclose(f);
}
