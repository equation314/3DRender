#include "common/config.h"
#include "common/const.h"
#include "engine/photonmapper/photonmap.h"
#include "engine/photonmapper/photonmapper.h"
#include "engine/photonmapper/photontracer.h"
#include "scene/camera.h"

void PhotonMapper::run(const std::string& outFile)
{
    if (!m_scene) return;

    cout << "Building photon map..." << endl;
    m_map = new PhotonMap();
    PhotonTracer* tracer = new PhotonTracer(m_scene, m_map);
    tracer->emitPhotons(Config::photon_emitted_number);
    delete tracer;

    m_map->build();
    RayTracer::run(outFile);
    delete m_map;
}

Color PhotonMapper::m_calcLocalIllumination(const Collision& coll, const Material* material, const Color& factor) const
{
    Color color = material->color * coll.object->getTextureColor(coll),
          ret = color * m_map->getIrradiance(coll, Config::photon_sampling_number) * (material->diff / Config::photon_emitted_number) * factor;
    if (Config::photon_map_only)
        return ret + color * m_scene->getAmbientLightColor() * factor * material->diff;
    else
        return ret + RayTracer::m_calcLocalIllumination(coll, material, factor);
}
