#ifndef PHOTONTRACER_H
#define PHOTONTRACER_H

struct Photon;
class Scene;
class PhotonMap;
class HitPointMap;

class PhotonTracer
{
public:
    // 用于普通光子映射
    PhotonTracer(Scene* scene, PhotonMap* map)
        : m_photon_map(map), m_hit_point_map(nullptr), m_scene(scene) {}

    // 用于 PPM
    PhotonTracer(Scene* scene, HitPointMap* map)
        : m_photon_map(nullptr), m_hit_point_map(map), m_scene(scene) {}

    ~PhotonTracer() {}

    // 发射光子
    void emitPhotons(int photonNumber);

private:
    PhotonMap* m_photon_map;
    HitPointMap* m_hit_point_map;
    Scene* m_scene;

    void m_photonTracing(Photon& photon, int depth, bool isInternal);
};

#endif // PHOTONTRACER_H
