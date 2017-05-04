#ifndef MATERIAL_H
#define MATERIAL_H

#include "common/color.h"

struct Material
{
public:
    Material() {}

    Color color;       // 颜色
    double diff, spec; // 漫反射系数，镜面反射系数
    double refl, refr; // 反射和折射光的比例
    double rindex;     // 折射率
};

#endif // MATERIAL_H
