#ifndef ROTATIONBODY_H
#define ROTATIONBODY_H

#include "common/vector3.h"
#include "object/object.h"
#include "util/beziercurve3.h"

#include <vector>

class RotationBody : public Object
{
public:
    typedef std::vector<BezierCurve3> Curves;

    RotationBody(const Vector3& o, const Curves& curves = {}, const Material* m = nullptr);
    RotationBody(const Json::Value& object);
    ~RotationBody();

    virtual std::string getType() const override { return "RotationBody"; }

    // 与视线相交
    virtual Collision collide(const Ray& ray) const override;

    // 交点处的纹理颜色
    virtual Color getTextureColor(const Collision& coll) const override;

    virtual Json::Value toJson() const override;

    // 设置纹理起点极角
    void setTextureArg(double a) { m_arg = a; }

    // 设置子曲面纹理比例
    void setTextureRatio(const std::vector<double>& ratios) { m_texture_ratios = ratios; }

    BezierCurve3 getCurve(int i) const { return m_curves[i]; }
    void addCurve(const BezierCurve3& curve) { m_curves.push_back(curve); }

    // 曲面上一点 P(u, v)
    Vector3 P(int i, double u, double v) const;

    // 保存为 OBJ 格式
    void saveOBJ(const std::string& file, int density) const;

private:
    Vector3 m_o;                                                // 底面中心点
    Curves m_curves;                                            // 曲线
    double m_r, m_h, m_arg;                                     // 包围圆柱体的底面半径、高，纹理起点极角
    std::vector<double> m_texture_ratios, m_texture_ratios_sum; // 每个子曲面纹理的比例，比例前缀和

    Cylinder* m_bounding_cylinder;          // 包围圆柱体
    std::vector<Cylinder*> m_sub_cylinders; // 子旋转面的包围圆柱体
    std::vector<uint64> m_identifiers;      // 每个子旋转面都有标识符

    void m_init();

    Vector3 m_dPdu(int i, double u, double v) const; // 对 u 的偏导数
    Vector3 m_dPdv(int i, double u, double v) const; // 对 v 的偏导数
};

#endif // ROTATIONBODY_H
