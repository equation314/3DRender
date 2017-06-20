#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "engine/engine.h"

class RayTracer : public Engine
{
public:
    RayTracer(Scene* scene)
        : Engine(scene) {}
    virtual ~RayTracer() {}

    virtual void run(const std::string& outFile) override;

protected:
    // 处理漫反射
    virtual Color m_calcLocalIllumination(const Collision& coll, const Material* Material, const Color& factor) const;

    // 跟踪光线
    Color m_rayTracing(const Vector3& start, const Vector3& dir,
                       const Color& factor, double weight, int depth, bool isInternal) const;

    // 采样颜色
    Color m_samplingColor(int ox, int oy) const;
};

#endif // RAYTRACER_H
