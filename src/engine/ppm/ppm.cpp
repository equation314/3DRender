#include "common/config.h"
#include "common/const.h"
#include "engine/photonmapper/photontracer.h"
#include "engine/ppm/hitpointmap.h"
#include "engine/ppm/ppm.h"
#include "scene/camera.h"

void PPM::run(const std::string& outFile)
{
    if (!m_scene) return;
    m_map = new HitPointMap();
    int w = m_camera->getW(), h = m_camera->getH();

    cout << "Eye tracing..." << endl;
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            m_pixel_x = i, m_pixel_y = j;
            if (!j) cout << "column " << i << endl;
            m_camera->setColor(i, j, m_samplingColor(i, j));
        }

    m_camera->print(outFile.c_str());
    m_map->build();

    Bmp* film = m_camera->copyFilm();

    cout << "Start iteration..." << endl;
    PhotonTracer* tracer = new PhotonTracer(m_scene, m_map);
    for (int i = 0, tot = 0; i < Config::ppm_iteration_depth; i++)
    {
        cout << "Round " << i << ":" << endl;
        tracer->emitPhotons(Config::ppm_photon_emitted_number);
        m_map->update();
        tot += Config::ppm_photon_emitted_number;

        m_camera->setFilm(film);
        for (auto p = m_map->hitPointBegin(); p != m_map->hitPointEnd(); p++)
        {
            Color color = m_camera->getColor(p->x, p->y);
            color += p->color * p->flux * (1 / Const::PI / p->r2 / tot);
            color = color.confine();
            m_camera->setColor(p->x, p->y, color);
        }
        m_camera->print(outFile.c_str());
    }

    delete film;
    delete tracer;
    delete m_map;
}

Color PPM::m_calcLocalIllumination(const Collision& coll, const Material* material, const Color& factor) const
{
    HitPoint point;
    point.pos = coll.p;
    point.n = coll.n;
    point.dir = coll.ray.dir;
    point.material = material;
    point.color = material->color * coll.object->getTextureColor(coll) * factor * material->diff;
    point.r2 = Config::ppm_initial_search_radius;
    point.x = m_pixel_x;
    point.y = m_pixel_y;
    m_map->addHitPoint(point);

    if (Config::photon_map_only)
        return Color();
    else
        return RayTracer::m_calcLocalIllumination(coll, material, factor);
}
