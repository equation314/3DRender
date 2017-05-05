#include "common/const.h"
#include "object/material.h"

bool Material::compare(const Material* B) const
{
    return this->refl + Const::EPS < B->refl ||
           (abs(this->refl - B->refl) < Const::EPS && this->refr + Const::EPS < B->refr);
}
