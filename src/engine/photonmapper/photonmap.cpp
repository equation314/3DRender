#include "common/const.h"
#include "engine/photonmapper/photonmap.h"
#include "object/collision.h"

#include <algorithm>

PhotonMap::~PhotonMap()
{
    if (m_photons) delete[] m_photons;
    if (m_plane) delete[] m_plane;
}

Color PhotonMap::getIrradiance(const Collision& coll, int samples)
{
    while (!m_pq.empty()) m_pq.pop();
    m_pq.push(make_pair(0.1, -1)); // 最大采样半径
    m_samples = samples, m_pos = coll.p;
    m_findNearestPhotons(0, m_n);

    Color ret;
    double maxDist = m_pq.top().first, k = 1;
    for (int i = 0; i < samples && !m_pq.empty(); i++, m_pq.pop())
    {
        int id = m_pq.top().second;
        if (id >= 0 && coll.n.dot(m_photons[id].dir) < -Const::EPS)
            ret += m_photons[id].pow * (1 - m_pq.top().first / maxDist / k);
    }
    return ret * (1 / Const::PI / maxDist / (1 - 2 / 3 / k));
}

void PhotonMap::build()
{
    if (m_photons) delete[] m_photons;
    if (m_plane) delete[] m_plane;

    m_n = m_tmp.size();
    m_photons = new Photon[m_n];
    for (int i = 0; i < m_n; i++) m_photons[i] = m_tmp[i];
    m_tmp.clear();
    m_plane = new unsigned char[m_n * 2];

    cout << "Total photons: " << m_n << endl;
    m_build(0, m_n);
}

void PhotonMap::m_findNearestPhotons(int l, int r)
{
    if (l >= r) return;
    int mi = (l + r) >> 1, k = m_plane[mi];

    double dist = m_pos[k] - m_photons[mi].pos[k], res = (m_photons[mi].pos - m_pos).mod2();
    if (res < m_pq.top().first)
    {
        if ((int) m_pq.size() == m_samples) m_pq.pop();
        m_pq.push(make_pair(res, mi));
    }

    if (dist < 0)
    {
        m_findNearestPhotons(l, mi);
        if (dist * dist < m_pq.top().first) m_findNearestPhotons(mi + 1, r);
    }
    else
    {
        m_findNearestPhotons(mi + 1, r);
        if (dist * dist < m_pq.top().first) m_findNearestPhotons(l, mi);
    }
}

void PhotonMap::m_build(int l, int r)
{
    if (l >= r) return;
    int mi = (l + r) >> 1, k;

    // 选择方差最小的坐标分量进行划分
    double mean[3] = {0}, var[3] = {0};
    for (int i = l; i < r; i++)
        for (int j = 0; j < 3; j++) mean[j] += m_photons[i].pos[j];
    mean[0] /= r - l, mean[1] /= r - l, mean[2] /= r - l;
    for (int i = l; i < r; i++)
        for (int j = 0; j < 3; j++) var[j] += (m_photons[i].pos[j] - mean[j]) * (m_photons[i].pos[j] - mean[j]);
    if (var[0] > var[1] && var[0] > var[2])
        k = 0;
    else if (var[1] > var[0] && var[1] > var[2])
        k = 1;
    else
        k = 2;
    m_plane[mi] = k;

    nth_element(m_photons + l, m_photons + mi, m_photons + r, [&](const Photon& x, const Photon& y) {
        return x.pos[k] < y.pos[k];
    });
    m_build(l, mi);
    m_build(mi + 1, r);
}
