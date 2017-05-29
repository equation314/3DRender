#ifndef BEZIERCURVE3_H
#define BEZIERCURVE3_H

#include "common/vector2.h"

struct BezierCurve3
{
public:
    BezierCurve3(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3)
        : P0(p0), P1(p1), P2(p2), P3(p3) {}
    BezierCurve3(const Json::Value& curve)
        : P0(curve[0]), P1(curve[1]), P2(curve[2]), P3(curve[3]) {}

    Vector2 P(double t) const;  // 曲线上一点 P(t)
    Vector2 dP(double t) const; // 曲线上一点 P(t) 的切向量 P'(t)

    Json::Value toJson() const;

    Vector2 P0, P1, P2, P3;
};

#endif // BEZIERCURVE3_H
