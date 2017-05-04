#include "common/bmp.h"

Bmp::Bmp(int w, int h)
    : m_w(w), m_h(h)
{
    m_data = new BmpColor*[w];
    for (int i = 0; i < m_w; i++)
        m_data[i] = new BmpColor[h];
}

Bmp::~Bmp()
{
    for (int i = 0; i < m_w; i++) delete[] m_data[i];
    delete[] m_data;
}

void Bmp::setColor(int x, int y, const Color& color)
{
    m_data[x][y] = BmpColor(color.r * 255 + 0.5, color.g * 255 + 0.5, color.b * 255 + 0.5);
}

void Bmp::save(const std::string& file) const
{
    FILE* f = fopen(file.c_str(), "wb");
    m_writeHead(f);
    m_writeData(f);
    fclose(f);
}

void Bmp::m_writeInt(int x, int bytes, FILE* f) const
{
    for (int i = 0; i < bytes; i++, x /= 256)
        fputc(x % 256, f);
}

void Bmp::m_writeHead(FILE* f) const
{
    fprintf(f, "BM");
    m_writeInt((m_w * 3 + m_w % 4) * m_h + 54, 4, f);
    m_writeInt(0, 2, f), m_writeInt(0, 2, f);
    m_writeInt(54, 4, f);

    m_writeInt(40, 4, f);
    m_writeInt(m_w, 4, f), m_writeInt(m_h, 4, f);
    m_writeInt(1, 2, f), m_writeInt(24, 2, f);
    for (int i = 0; i < 24; i++) fputc(0, f);
}

void Bmp::m_writeData(FILE* f) const
{
    for (int j = m_h - 1; j >= 0; j--)
    {
        for (int i = 0; i < m_w; i++)
        {
            fputc(m_data[i][j].b, f);
            fputc(m_data[i][j].g, f);
            fputc(m_data[i][j].r, f);
        }
        for (int i = 0; i < m_w % 4; i++) fputc(0, f);
    }
}
