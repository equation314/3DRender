#ifndef MATERIAL_H
#define MATERIAL_H

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

    Color color, absorb_color; // 颜色
    double diff, spec;         // 漫反射系数，镜面反射系数
    double refl, refr;         // 反射和折射光的比例
    double rindex;             // 折射率

    // 求交时的优先级
    bool compare(const Material* B) const;
};

#endif // MATERIAL_H
