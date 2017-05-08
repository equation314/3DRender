#include "common/const.h"
#include "object/material.h"

Material::Material(const Color& c, double d, double s)
    : color(c), diff(d), spec(s), refl(0), refr(0), rindex(1),
      m_texture(nullptr), m_texture_func(nullptr)
{
}

Material::Material(const Color& c, double d, double s, double rl)
    : color(c), diff(d), spec(s), refl(rl), refr(0), rindex(1),
      m_texture(nullptr), m_texture_func(nullptr)
{
}

Material::Material(const Color& c, double d, double s, double rl, double rr, double ri, const Color& absorb)
    : color(c), absorb_color(absorb), diff(d), spec(s), refl(rl), refr(rr), rindex(ri),
      m_texture(nullptr), m_texture_func(nullptr)
{
}

Material::~Material()
{
    if (m_texture) delete m_texture;
}

Color Material::getTextureColor(double u, double v) const
{
    if (m_texture)
        return m_texture->getColor(u, v);
    else if (m_texture_func)
        return m_texture_func(u, v);
    else
        return Color(1, 1, 1);
}

bool Material::compare(const Material* B) const
{
    return this->refl + Const::EPS < B->refl ||
           (abs(this->refl - B->refl) < Const::EPS && this->refr + Const::EPS < B->refr);
}
