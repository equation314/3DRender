#ifndef MATERIAL_H
#define MATERIAL_H

#include "common/bmp.h"
#include "common/color.h"

struct Material
{
public:
    // 漫反射材质
    Material(const Color& c, double d, double s);

    // 不透明反射材质
    Material(const Color& c, double d, double s, double rl);

    // 透明材质
    Material(const Color& c, double d, double s, double rl, double rr, double ri, const Color& absorb);

    ~Material();

    Color color, absorb_color; // 颜色，透射时吸收的颜色
    double diff, spec;         // 漫反射系数，镜面反射系数
    double refl, refr;         // 反射和折射光的比例
    double rindex;             // 折射率

    bool hasTexture() const { return m_texture || m_texture_func; }

    void setTexture(Bmp* texture) { m_texture_func = nullptr, m_texture = texture; }
    void setTexture(Color (*func)(double, double))
    {
        if (m_texture) delete m_texture;
        m_texture = nullptr, m_texture_func = func;
    }

    // 获得纹理颜色
    Color getTextureColor(double u, double v) const;

    // 求交时的优先级
    bool compare(const Material* B) const;

private:
    Bmp* m_texture;                          // 纹理图片
    Color (*m_texture_func)(double, double); // 纹理函数
};

#endif // MATERIAL_H
