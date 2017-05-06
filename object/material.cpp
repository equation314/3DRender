#include "common/const.h"
#include "object/material.h"

Material::Material(const Color& c, double d, double s)
    : color(c), diff(d), spec(s), refl(0), refr(0), rindex(1)
{
}

Material::Material(const Color& c, double d, double s, double rl)
    : color(c), diff(d), spec(s), refl(rl), refr(0), rindex(1)
{
}

Material::Material(const Color& c, double d, double s, double rl, double rr, double ri, const Color& absorb)
    : color(c), absorb_color(absorb), diff(d), spec(s), refl(rl), refr(rr), rindex(ri)
{
}

bool Material::compare(const Material* B) const
{
    return this->refl + Const::EPS < B->refl ||
           (abs(this->refl - B->refl) < Const::EPS && this->refr + Const::EPS < B->refr);
}
