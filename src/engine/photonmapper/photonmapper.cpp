#include "common/config.h"
#include "common/const.h"
#include "engine/photonmapper/photonmap.h"
#include "engine/photonmapper/photonmapper.h"
#include "engine/photonmapper/photontracer.h"
#include "scene/camera.h"

#include <ctime>

void PhotonMapper::run(const std::string& outFile)
{
    if (!m_scene) return;

    cout << "Building photon map..." << endl;
    PhotonTracer* tracer = new PhotonTracer(m_scene);
    m_map = tracer->getPhotonMap();
    delete tracer;

    RayTracer::run(outFile);

    delete m_map;
}

Color PhotonMapper::m_calcLocalIllumination(const Collision& coll, const Material* material) const
{
    Color color = material->color * coll.object->getTextureColor(coll),
          ret = color * m_map->getIrradiance(coll, Config::photon_samples) * material->diff;
    if (Config::photon_map_only)
        return ret + color * m_scene->getAmbientLightColor() * material->diff;
    else
        return ret + RayTracer::m_calcLocalIllumination(coll, material) / 4;
}
