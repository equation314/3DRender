#ifndef PHOTON_H
#define PHOTON_H

#include "common/color.h"
#include "common/vector3.h"

struct Photon
{
public:
    Photon()
        : pos(), dir(), pow(0) {}
    Photon(const Vector3& p, const Vector3& d, const Color& power)
        : pos(p), dir(d), pow(power) {}

    Vector3 pos, dir; // 光子位置，方向
    Color pow;        // 光子能量，即光通量 Phi
};

#endif // PHOTON_H
