#include "curve/beziercurve3.h"

BezierCurve3::BezierCurve3(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3)
    : P0(p0), P1(p1), P2(p2), P3(p3),
      L(std::min(std::min(std::min(P0.x, P1.x), P2.x), P3.x)),
      R(std::max(std::max(std::max(P0.x, P1.x), P2.x), P3.x)),
      D(std::min(std::min(std::min(P0.y, P1.y), P2.y), P3.y)),
      U(std::max(std::max(std::max(P0.y, P1.y), P2.y), P3.y))
{
}

BezierCurve3::BezierCurve3(const Json::Value& curve)
    : P0(curve[0]), P1(curve[1]), P2(curve[2]), P3(curve[3]),
      L(std::min(std::min(std::min(P0.x, P1.x), P2.x), P3.x)),
      R(std::max(std::max(std::max(P0.x, P1.x), P2.x), P3.x)),
      D(std::min(std::min(std::min(P0.y, P1.y), P2.y), P3.y)),
      U(std::max(std::max(std::max(P0.y, P1.y), P2.y), P3.y))
{
}

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
