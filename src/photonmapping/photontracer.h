#ifndef PHOTONTRACER_H
#define PHOTONTRACER_H

#include "photonmapping/photonmap.h"

class Scene;

class PhotonTracer
{
public:
    PhotonTracer()
        : m_map(nullptr), m_scene(nullptr) {}
    ~PhotonTracer() {}

    PhotonMap* getPhotonMap(Scene* scene, int photonNumber);

private:
    PhotonMap* m_map;
    Scene* m_scene;

    void m_photonTracing(Photon& photon, int depth, bool isInternal);
};

#endif // PHOTONTRACER_H
