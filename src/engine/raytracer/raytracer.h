#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "engine/engine.h"

class RayTracer : public Engine
{
public:
    RayTracer()
        : Engine() {}
    ~RayTracer() {}

    virtual void run(Scene* scene, const std::string& outFile) override;

private:
    Color m_calcLocalIllumination(const Collision& coll, const Material* Material) const;
    Color m_calcReflection(const Collision& coll, const Material* Material, double weight, int depth, bool isInternal) const;
    Color m_calcRefraction(const Collision& coll, const Material* Material, double weight, int depth, bool isInternal) const;
    Color m_rayTraceing(const Vector3& start, const Vector3& dir, double weight, int depth, bool isInternal) const;
};

#endif // RAYTRACER_H
