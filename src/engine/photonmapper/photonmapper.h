#ifndef PHOTONMAPPER_H
#define PHOTONMAPPER_H

#include "engine/engine.h"

class PhotonMap;
class PhotonTracer;

class PhotonMapper : public Engine
{
public:
    PhotonMapper()
        : Engine() {}
    ~PhotonMapper() {}

    virtual void run(Scene* scene, const std::string& outFile) override;

private:
    PhotonMap* m_map;

    Color m_calcLocalIllumination(const Collision& coll, const Material* Material) const;
    Color m_calcReflection(const Collision& coll, const Material* Material, double weight, int depth, bool isInternal) const;
    Color m_calcRefraction(const Collision& coll, const Material* Material, double weight, int depth, bool isInternal) const;
    Color m_rayTraceing(const Vector3& start, const Vector3& dir, double weight, int depth, bool isInternal) const;
};

#endif // PHOTONMAPPER_H
