#ifndef PHOTONMAP_H
#define PHOTONMAP_H

#include "common/photon.h"

#include <queue>
#include <vector>

class Collision;

class PhotonMap
{
public:
    PhotonMap()
        : m_n(0), m_plane(nullptr), m_photons(nullptr) {}
    ~PhotonMap();

    // 加入光子
    void addPhoton(const Photon& photon) { m_tmp.push_back(photon); }

    // 获得交点处的辉度
    Color getIrradiance(const Collision& coll, int samples);

    void build();

private:
    int m_n;
    unsigned char* m_plane;
    Photon* m_photons;
    std::vector<Photon> m_tmp;
    std::priority_queue<pair<double, int>> m_pq;

    int m_samples; // 最近的光子数
    Vector3 m_pos; // 询问点

    // 建 KD-tree
    void m_build(int l, int r);

    // 找最近 m_samples 个光子
    void m_findNearestPhotons(int l, int r);
};

#endif // PHOTONMAP_H
