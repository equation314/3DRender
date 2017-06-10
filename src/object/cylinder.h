#ifndef CYLINDER_H
#define CYLINDER_H

#include "common/vector3.h"
#include "object/object.h"

class Cylinder : public Object
{
public:
    Cylinder(const Vector3& o, double r, double h, const Material* m = nullptr);
    Cylinder(const Json::Value& object);

    virtual std::string getType() const override { return "Cylinder"; }

    // 与视线相交
    virtual Collision collide(const Vector3& start, const Vector3& dir) const override;

    // 交点处的纹理颜色
    virtual Color getTextureColor(const Collision& coll) const override;

    virtual Json::Value toJson() const override;

    // 设置纹理起点极角
    void setTextureArg(double a) { m_arg = a; }

private:
    Vector3 m_o;            // 底面圆心
    double m_r, m_h, m_arg; // 底面半径，高，纹理起点极角
};

#endif // CYLINDER_H
