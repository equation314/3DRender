#ifndef HITPOINTMAP_H
#define HITPOINTMAP_H

#include "common/photon.h"
#include "object/material.h"

#include <vector>

struct HitPoint
{
    HitPoint()
        : N(0), M(0), flux() {}

    Vector3 pos, n, dir;      // 交点位置，法向，视线方向
    const Material* material; // 材质
    int x, y;                 // 像素点位置
    Color color;              // 材质原始颜色

    double r2;  // 采样半径的平方
    int N, M;   // 累计光子数，本轮发射累计光子数
    Color flux; // 光通量

    void update(const Photon& photon)
    {
        M++;
        if (photon.dir.dot(n) < -Const::EPS)
            flux += photon.pow * material->BRDF(-photon.dir, n, dir);
    }
};

class HitPointMap
{
public:
    HitPointMap()
        : m_n(0), m_plane(nullptr), m_nodes(nullptr) {}
    ~HitPointMap();

    std::vector<HitPoint>::const_iterator hitPointBegin() const { return m_points.begin(); }
    std::vector<HitPoint>::const_iterator hitPointEnd() const { return m_points.end(); }

    // 加入 hit point
    void addHitPoint(const HitPoint& point) { m_points.push_back(point); }

    // 根据光子更新 hit point
    void incomingPhoton(const Photon& photon);

    // 建 KD-tree
    void build();

    // 光子发射完后更新 hit point
    void update();

private:
    struct Node
    {
        Node()
            : point(nullptr) {}
        Node(HitPoint* p)
            : point(p)
        {
            double r = sqrt(p->r2);
            x1 = p->pos.x - r, x2 = p->pos.x + r;
            y1 = p->pos.y - r, y2 = p->pos.y + r;
            z1 = p->pos.z - r, z2 = p->pos.z + r;
        }

        // 根据子节点更新包围盒
        void updateBoundingBox(Node* child)
        {
            x1 = std::min(x1, child->x1), x2 = std::max(x2, child->x2);
            y1 = std::min(y1, child->y1), y2 = std::max(y2, child->y2);
            z1 = std::min(z1, child->z1), z2 = std::max(z2, child->z2);
        }

        HitPoint* point;
        double x1, x2, y1, y2, z1, z2; // bounding box
    };

    int m_n;
    unsigned char* m_plane;
    Node* m_nodes;
    std::vector<HitPoint> m_points;

    Photon m_photon; // 进入的光子
    Vector3 m_pos;   // 光子位置

    // 建 KD-tree
    void m_build(int l, int r);

    // 更新 r2 后需重新建树来更新 bounding box
    void m_rebuild(int l, int r);

    // 找所有距离不超过 sqrt(r2) 的 hit point
    void m_findNearHitPoints(int l, int r);
};

#endif // HITPOINTMAP_H
