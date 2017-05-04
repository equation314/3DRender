#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "common/vector3.h"
#include "light/light.h"

class PointLight : public Light
{
public:
    PointLight(const Color& c, const Vector3& o)
        : Light(c), m_o(o) {}

    virtual Vector3 getSource() const { return m_o; }

    virtual Collision collide(const Vector3& start, const Vector3& dir) const override;

private:
    Vector3 m_o;
};

#endif // POINTLIGHT_H
