#ifndef MATERIAL_H
#define MATERIAL_H

#include "common/color.h"

struct Material
{
public:
    Material(const Color& c, double d, double s, double rl, double rr, double ri = 0)
        : color(c), diff(d), spec(s), refl(rl), refr(rr), rindex(ri) {}

    Color color;       // 颜色
    double diff, spec; // 漫反射系数，镜面反射系数
    double refl, refr; // 反射和折射光的比例
    double rindex;     // 折射率

    // 求交时的优先级
    bool compare(const Material* B) const;
};

#endif // MATERIAL_H
