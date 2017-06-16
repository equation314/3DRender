#include "common/const.h"
#include "light/light.h"
#include "object/object.h"
#include "photonmapping/photonmap.h"
#include "photonmapping/photonmapper.h"
#include "photonmapping/photontracer.h"
#include "scene/camera.h"
#include "scene/scene.h"

const double MIN_WEIGHT = 0.05;
const int MAX_DEPTH = 20;
const int SPEC_POWER = 50;
const int PHOTON_NUMBER = 1000000;
const int PHOTON_SAMPLES = 500;

void PhotonMapper::run(Scene* scene, const std::string& outFile)
{
    m_scene = scene;
    Camera* camera = scene->getCamera();
    int w = camera->getW(), h = camera->getH();

    PhotonTracer* tracer = new PhotonTracer();
    m_map = tracer->getPhotonMap(scene, PHOTON_NUMBER);
    delete tracer;

    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            if (!j) cout << "column " << i << endl;
            Vector3 dir = camera->emit(i, j);
            Color color = m_rayTraceing(camera->getEye(), dir, 1, 1, false);
            camera->setColor(i, j, color);
        }

    delete m_map;
    camera->print(outFile.c_str());
}

Color PhotonMapper::m_calcLocalIllumination(const Collision& coll, const Material* material) const
{
    Vector3 r = coll.ray_dir.reflect(coll.n);
    Color color = material->color * coll.object->getTextureColor(coll);
    Color ret = color * (m_scene->getAmbientLightColor() + m_map->getIrradiance(coll, PHOTON_SAMPLES)) * material->diff;

    for (auto light = m_scene->lightsBegin(); light != m_scene->lightsEnd(); light++)
    {
        Vector3 l = (*light)->getSource() - coll.p;
        double d = l.mod2();
        l = l.unitize();
        double f = l.dot(coll.n);
        if (f < Const::EPS) continue;
        double shade = (*light)->getShadowRatio(m_scene, coll.p);
        if (shade < Const::EPS) continue;

        if (material->diff > Const::EPS) // 漫反射
            ret += color * (*light)->getColor() * (material->diff * f * shade) / d;
        if (material->spec > Const::EPS) // 镜面反射
            ret += color * (*light)->getColor() * (material->spec * pow(l.dot(r), SPEC_POWER)) / d;
    }
    return ret;
}

Color PhotonMapper::m_rayTraceing(const Vector3& start, const Vector3& dir, double weight, int depth, bool isInternal) const
{
    if (weight < MIN_WEIGHT) return m_scene->getAmbientLightColor();
    Collision coll = m_scene->findNearestCollision(start, dir);
    if (!coll.isHit())
        return m_scene->getAmbientLightColor();
    else if (coll.atLight())
        return coll.light->getColor();
    else if (depth <= MAX_DEPTH)
    {
        Color ret;
        const Object* obj = coll.object;
        const Material* material = obj->getMaterial();

        if (material->diff > Const::EPS || material->spec > Const::EPS)
            ret += m_calcLocalIllumination(coll, material);
        if (material->refl > Const::EPS || material->refr > Const::EPS)
        {
            double n = material->rindex;
            if (isInternal) n = 1 / n;
            Vector3 refl = coll.ray_dir.reflect(coll.n);
            Vector3 refr = coll.ray_dir.refract(coll.n, n);
            if (material->refr < Const::EPS) // 全镜面反射
                ret += m_rayTraceing(coll.p, refl, weight * material->refl, depth + 1, isInternal) * (material->color * material->refl);
            else if (refr.mod2() < Const::EPS) // 全反射
            {
                double k = material->refl + material->refr;
                ret += m_rayTraceing(coll.p, refl, weight * k, depth + 1, isInternal) * (material->color * k);
            }
            else if (material->refl < Const::EPS) // 全透射
                ret += m_rayTraceing(coll.p, refr, weight * material->refr, depth + 1, !isInternal) * material->refr;
            else
            {
                // Fresnel equations
                double cosI = -coll.ray_dir.dot(coll.n), cosT = sqrt(1 - (1 - cosI * cosI) / n / n);
                double r1 = (cosI * n - cosT) / (cosI * n + cosT),
                       r2 = (cosI - cosT * n) / (cosI + cosT * n);
                double kl = (r1 * r1 + r2 * r2) / 2, kr = 1 - kl;

                if (kl > Const::EPS) ret += m_rayTraceing(coll.p, refl, weight * kl, depth + 1, isInternal) * (material->color * kl);
                if (kr > Const::EPS) ret += m_rayTraceing(coll.p, refr, weight * kr, depth + 1, !isInternal) * kr;
            }
        }

        // 透明材质的颜色过滤
        if (isInternal)
            ret *= (material->absorb_color * -coll.dist).exp();

        return ret.confine();
    }
    else
        return Color();
}
