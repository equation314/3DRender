#ifndef ROTATIONBODY_H
#define ROTATIONBODY_H

#include "common/vector3.h"
#include "curve/beziercurve3.h"
#include "object/object.h"

#include <vector>

class RotationBody : public Object
{
public:
    typedef std::vector<BezierCurve3> Curves;

    RotationBody(const Material* m, const Vector3& o, const Curves& curves = {});
    RotationBody(const Json::Value& object);

    // 与视线相交
    virtual Collision collide(const Vector3& start, const Vector3& dir) const override;

    // 某点的纹理颜色
    virtual Color getTextureColor(const Vector3& p) const override;

    virtual Json::Value toJson() const override;

    BezierCurve3 getCurve(int i) const { return m_curves[i]; }
    void addCurve(const BezierCurve3& curve) { m_curves.push_back(curve); }

    // 曲面上一点 P(u, v)
    Vector3 P(double u, double v) const;

    // 保存为 OBJ 格式
    void saveOBJ(const std::string& file, int density) const;

private:
    Vector3 m_o;     // 底面中心点
    Curves m_curves; // 曲线
};

#endif // ROTATIONBODY_H
