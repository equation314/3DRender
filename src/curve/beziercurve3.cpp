#include "curve/beziercurve3.h"

Vector2 BezierCurve3::P(double t) const
{
    double tt = t * t, ttt = tt * t;
    double s = 1 - t, ss = s * s, sss = ss * s;
    return P0 * sss + P1 * (3 * ss * t) + P2 * (3 * s * tt) + P3 * ttt;
}

Vector2 BezierCurve3::dP(double t) const
{
    double d = 3 * t * t,
           a = -d + 6 * t - 3,
           b = d * 3 - 12 * t + 3,
           c = -a - b - d;
    return P0 * a + P1 * b + P2 * c + P3 * d;
}

Json::Value BezierCurve3::toJson() const
{
    Json::Value curve;
    curve.append(P0.toJson());
    curve.append(P1.toJson());
    curve.append(P2.toJson());
    curve.append(P3.toJson());
    return curve;
}
