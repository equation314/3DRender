#ifndef PHOTONMAPPER_H
#define PHOTONMAPPER_H

#include "engine/raytracer/raytracer.h"

class PhotonMap;
class PhotonTracer;

class PhotonMapper : public RayTracer
{
public:
    PhotonMapper(Scene* scene)
        : RayTracer(scene) {}
    virtual ~PhotonMapper() {}

    virtual void run(const std::string& outFile) override;

private:
    PhotonMap* m_map;

    Color m_calcLocalIllumination(const Collision& coll, const Material* Material) const override;
};

#endif // PHOTONMAPPER_H
