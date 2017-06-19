#ifndef PHOTONTRACER_H
#define PHOTONTRACER_H

#include "engine/photonmapper/photonmap.h"

class Scene;

class PhotonTracer
{
public:
    PhotonTracer(Scene* scene)
        : m_map(nullptr), m_scene(scene) {}
    ~PhotonTracer() {}

    PhotonMap* getPhotonMap();

private:
    PhotonMap* m_map;
    Scene* m_scene;

    void m_photonTracing(Photon& photon, int depth, bool isInternal);
};

#endif // PHOTONTRACER_H
