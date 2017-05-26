#include "common/const.h"
#include "object/material.h"

Material::Material()
    : color(1, 1, 1), absorb_color(0, 0, 0), diff(0.8), spec(0.2), refl(0), refr(0), rindex(1),
      m_texture(nullptr), m_texture_func(nullptr)
{
}

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

Material::Material(const Json::Value material)
    : color(material["color"]), absorb_color(material["absorb_color"]),
      diff(material["diff"].asDouble()), spec(material["spec"].asDouble()), refl(material["refl"].asDouble()),
      refr(material["refr"].asDouble()), rindex(material["refr_index"].asDouble()),
      m_texture(nullptr), m_texture_func(nullptr)
{
    if (material["texture"].isString())
        m_texture = new Bmp(material["texture"].asString());
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

Json::Value Material::toJson() const
{
    Json::Value material;
    material["color"] = color.toJson();
    material["diff"] = diff;
    material["spec"] = spec;
    if (refl > Const::EPS) material["refl"] = refl;
    if (refr > Const::EPS)
    {
        material["refr"] = refr;
        material["refr_index"] = rindex;
        material["absorb_color"] = absorb_color.toJson();
    }
    if (m_texture)
        material["texture"] = m_texture->getFilename();
    else if (m_texture_func)
        material["texture_func"] = "Function";
    return material;
}
