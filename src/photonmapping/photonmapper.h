#ifndef PHOTONMAPPER_H
#define PHOTONMAPPER_H

#include "common/color.h"
#include "common/vector3.h"

class Collision;
class Material;
class Scene;
class PhotonMap;
class PhotonTracer;

class PhotonMapper
{
public:
    PhotonMapper() {}
    ~PhotonMapper() {}

    void run(Scene* scene, const std::string& outFile);

private:
    Scene* m_scene;
    PhotonMap* m_map;

    Color m_calcLocalIllumination(const Collision& coll, const Material* Material) const;
    Color m_calcReflection(const Collision& coll, const Material* Material, double weight, int depth, bool isInternal) const;
    Color m_calcRefraction(const Collision& coll, const Material* Material, double weight, int depth, bool isInternal) const;
    Color m_rayTraceing(const Vector3& start, const Vector3& dir, double weight, int depth, bool isInternal) const;
};

#endif // PHOTONMAPPER_H
