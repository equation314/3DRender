#include "common/config.h"
#include "common/const.h"
#include "engine/ppm/hitpointmap.h"

#include <algorithm>
#include <iostream>

HitPointMap::~HitPointMap()
{
    if (m_nodes) delete[] m_nodes;
    if (m_plane) delete[] m_plane;
    m_points.clear();
}

void HitPointMap::incomingPhoton(const Photon& photon)
{
    m_findNearHitPoints(0, m_n, photon);
}

void HitPointMap::build()
{
    if (m_nodes) delete[] m_nodes;
    if (m_plane) delete[] m_plane;

    m_n = m_points.size();
    m_plane = new unsigned char[m_n * 2];
    m_nodes = new Node[m_n * 2];

    std::cout << "Total hit points: " << m_n << std::endl;
    m_build(0, m_n);
}

void HitPointMap::update()
{
    for (auto& point : m_points)
        if (point.M)
        {
            double k = (point.N + point.M * Config::ppm_alpha) / (point.N + point.M);
            point.r2 *= k;
            point.flux *= k;
            point.N += point.M * Config::ppm_alpha;
            point.M = 0;
        }
    m_rebuild(0, m_n);
}

void HitPointMap::m_findNearHitPoints(int l, int r, const Photon& photon)
{
    if (l >= r) return;
    int mi = (l + r) >> 1, k = m_plane[mi];
    Vector3 pos = photon.pos;

    if (pos.x < m_nodes[mi].x1 || pos.x > m_nodes[mi].x2 ||
        pos.y < m_nodes[mi].y1 || pos.y > m_nodes[mi].y2 ||
        pos.z < m_nodes[mi].z1 || pos.z > m_nodes[mi].z2) return;

    if ((pos - m_nodes[mi].point->pos).mod2() <= m_nodes[mi].point->r2)
        m_nodes[mi].point->update(photon);

    if (pos[k] - m_nodes[mi].point->pos[k] < 0)
    {
        m_findNearHitPoints(l, mi, photon);
        m_findNearHitPoints(mi + 1, r, photon);
    }
    else
    {
        m_findNearHitPoints(mi + 1, r, photon);
        m_findNearHitPoints(l, mi, photon);
    }
}

void HitPointMap::m_build(int l, int r)
{
    if (l >= r) return;
    int mi = (l + r) >> 1, k;

    // 选择方差最小的坐标分量进行划分
    double mean[3] = {0}, var[3] = {0};
    for (int i = l; i < r; i++)
        for (int j = 0; j < 3; j++) mean[j] += m_points[i].pos[j];
    mean[0] /= r - l, mean[1] /= r - l, mean[2] /= r - l;
    for (int i = l; i < r; i++)
        for (int j = 0; j < 3; j++) var[j] += (m_points[i].pos[j] - mean[j]) * (m_points[i].pos[j] - mean[j]);
    if (var[0] > var[1] && var[0] > var[2])
        k = 0;
    else if (var[1] > var[0] && var[1] > var[2])
        k = 1;
    else
        k = 2;

    nth_element(m_points.begin() + l, m_points.begin() + mi, m_points.begin() + r, [&](const HitPoint& x, const HitPoint& y) {
        return x.pos[k] < y.pos[k];
    });

    m_plane[mi] = k, m_nodes[mi] = Node(&m_points[mi]);
    if (l < mi)
    {
        m_build(l, mi);
        m_nodes[mi].updateBoundingBox(&m_nodes[(l + mi) >> 1]);
    }
    if (mi + 1 < r)
    {
        m_build(mi + 1, r);
        m_nodes[mi].updateBoundingBox(&m_nodes[(mi + 1 + r) >> 1]);
    }
}

void HitPointMap::m_rebuild(int l, int r)
{
    if (l >= r) return;
    int mi = (l + r) >> 1;

    m_nodes[mi] = Node(&m_points[mi]);
    if (l < mi)
    {
        m_rebuild(l, mi);
        m_nodes[mi].updateBoundingBox(&m_nodes[(l + mi) >> 1]);
    }
    if (mi + 1 < r)
    {
        m_rebuild(mi + 1, r);
        m_nodes[mi].updateBoundingBox(&m_nodes[(mi + 1 + r) >> 1]);
    }
}
