#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "scene.h"

class RayTracer
{
public:
    RayTracer() {}
    ~RayTracer() {}

    void run(Scene* scene);

private:
    Scene* m_scene;

    Color m_calcLocalIllumination(const Collision& coll, Material* Material) const;
    Color m_calcReflection(const Collision& coll, Material* Material, double weight, int depth) const;
    Color m_calcRefraction(const Collision& coll, Material* Material, double weight, int depth) const;
    Color m_rayTraceing(const Vector3& start, const Vector3& dir, double weight, int depth) const;
};

#endif // RAYTRACER_H
