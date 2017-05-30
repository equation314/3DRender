#ifndef SPHERE_H
#define SPHERE_H

#include "common/vector3.h"
#include "object/object.h"

class Sphere : public Object
{
public:
    Sphere(const Vector3& o, double r, const Material* m = nullptr);
    Sphere(const Json::Value& object);

    virtual std::string getType() const override { return "Sphere"; }

    // 与视线相交
    virtual Collision collide(const Vector3& start, const Vector3& dir) const override;

    // 某点的纹理颜色
    virtual Color getTextureColor(const Vector3& p) const override;

    // 设置纹理坐标轴
    void setTextureAxis(const Vector3& dz, const Vector3& dx) { m_dz = dz, m_dx = dx; }

    virtual Json::Value toJson() const override;

private:
    Vector3 m_o;        // 球心
    double m_r;         // 半径
    Vector3 m_dz, m_dx; // 北极向量和 0 度经线方向
};

#endif // SPHERE_H
