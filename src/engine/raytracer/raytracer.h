#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "common/ray.h"
#include "engine/engine.h"

class RayTracer : public Engine
{
public:
    RayTracer(Scene* scene);
    virtual ~RayTracer();

    virtual void run(const std::string& outFile) override;

protected:
    uint64** m_hash;          // 光线经过物体的 HASH 值，优化 SSAA
    bool** m_is_edge;         // 是否是物体边缘
    int m_pixel_x, m_pixel_y; // 每次跟踪时像素的位置

    // 处理漫反射
    virtual Color m_calcLocalIllumination(const Collision& coll, const Material* Material, const Color& factor) const;

    // 跟踪光线
    Color m_rayTracing(const Ray& ray, const Color& factor, double weight, int depth, bool isInternal) const;

    // 景深采样
    Color m_DOFSamplingColor(double ox, double oy, double factor = 1) const;

    // 抗锯齿采样
    Color m_AASamplingColor(int ox, int oy);
};

#endif // RAYTRACER_H
