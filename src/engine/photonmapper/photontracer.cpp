#include "common/const.h"
#include "engine/photonmapper/photontracer.h"
#include "light/light.h"
#include "object/material.h"
#include "object/object.h"
#include "scene/scene.h"

const int MAX_DEPTH = 10;

PhotonMap* PhotonTracer::getPhotonMap(Scene* scene, int photonNumber)
{
    if (m_map) delete m_map;
    m_scene = scene;
    m_map = new PhotonMap();
    double power = 0;

    for (auto l = scene->lightsBegin(); l != scene->lightsEnd(); l++)
        power += (*l)->getColor().power();
    power /= photonNumber;

    for (auto l = scene->lightsBegin(); l != scene->lightsEnd(); l++)
    {
        double lightPower = (*l)->getColor().power();
        for (; lightPower > 0; lightPower -= power)
        {
            Photon photon = (*l)->emitPhoton(power);
            m_photonTracing(photon, 1, false);
        }
    }

    m_map->build();
    return m_map;
}

void PhotonTracer::m_photonTracing(Photon& photon, int depth, bool isInternal)
{
    Collision coll = m_scene->findNearestCollision(photon.pos, photon.dir);
    if (coll.isHit() && coll.atObject() && depth <= MAX_DEPTH)
    {
        photon.pos = coll.p;
        const Object* obj = coll.object;
        const Material* material = obj->getMaterial();
        if (material->diff > Const::EPS) m_map->addPhoton(photon);

        Color cd = material->color * obj->getTextureColor(coll), ct(1, 1, 1);
        if (isInternal) // 透明材质的颜色过滤
        {
            Color absorb = (material->absorb_color * -coll.dist).exp();
            cd *= absorb, ct *= absorb;
        }
        double fortune = Const::randDouble();
        double pd = (material->diff + material->spec) * cd.power(), // 漫反射概率
            ps = material->refl * cd.power(),                       // 镜面反射概率
            pt = material->refr * ct.power();                       // 透射概率

        if (fortune < pd) // 漫反射
        {
            photon.dir = coll.n.diffuse();
            photon.pow *= cd / cd.power();
            m_photonTracing(photon, depth + 1, isInternal);
        }
        else if (fortune < pd + ps) // 镜面反射
        {
            photon.dir = coll.ray_dir.reflect(coll.n);
            photon.pow *= cd / cd.power();
            m_photonTracing(photon, depth + 1, isInternal);
        }
        else if (fortune < pd + ps + pt) // 透射
        {
            double n = material->rindex;
            if (isInternal) n = 1 / n;
            photon.dir = coll.ray_dir.refract(coll.n, n);
            photon.pow *= ct / ct.power();
            m_photonTracing(photon, depth + 1, !isInternal);
        }
    }
}
