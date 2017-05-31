#include "common/const.h"
#include "light/light.h"
#include "object/object.h"
#include "raytracing/raytracer.h"
#include "scene/camera.h"
#include "scene/scene.h"

const double MIN_WEIGHT = 0.05;
const int MAX_DEPTH = 5;
const int SPEC_POWER = 50;

void RayTracer::run(Scene* scene, const std::string& outFile)
{
    m_scene = scene;
    Camera* camera = scene->getCamera();
    int w = camera->getW(), h = camera->getH();

    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            if (!j) cout << "column " << i << endl;
            Vector3 dir = camera->emit(i, j);
            Color color = m_rayTraceing(camera->getEye(), dir, 1, 1, false);
            camera->setColor(i, j, color);
        }

    camera->print(outFile.c_str());
}

Color RayTracer::m_calcLocalIllumination(const Collision& coll, const Material* material) const
{
    Vector3 r = coll.ray_dir.reflect(coll.n);
    Color color = material->color * coll.object->getTextureColor(coll);
    Color ret = color * m_scene->getAmbientLightColor() * material->diff; // 环境光
    for (auto light = m_scene->lightsBegin(); light != m_scene->lightsEnd(); light++)
    {
        Vector3 l = ((*light)->getSource() - coll.p).unitize();
        double f = l.dot(coll.n);
        if (f < Const::EPS) continue;
        double shade = (*light)->getShadowRatio(m_scene, coll.p);
        if (shade < Const::EPS) continue;

        if (material->diff > Const::EPS) // 漫反射
            ret += color * (*light)->getColor() * (material->diff * f * shade);
        if (material->spec > Const::EPS) // 镜面反射
            ret += color * (*light)->getColor() * (material->spec * pow(l.dot(r), SPEC_POWER));
    }
    return ret;
}

Color RayTracer::m_calcReflection(const Collision& coll, const Material* material, double weight, int depth, bool isInternal) const
{
    Vector3 r = coll.ray_dir.reflect(coll.n);
    Color ret = m_rayTraceing(coll.p, r, weight * material->refl, depth + 1, isInternal);
    return ret * (material->color * material->refl);
}

Color RayTracer::m_calcRefraction(const Collision& coll, const Material* material, double weight, int depth, bool isInternal) const
{
    double rindex = material->rindex;
    if (isInternal) rindex = 1 / rindex;
    Vector3 r = coll.ray_dir.refract(coll.n, rindex);
    if (r.mod2() < Const::EPS) return Color();

    Color ret = m_rayTraceing(coll.p, r, weight * material->refr, depth + 1, !isInternal);
    if (!isInternal)
        return ret * material->refr;
    else
        return ret * (material->absorb_color * -coll.dist).exp();
}

Color RayTracer::m_rayTraceing(const Vector3& start, const Vector3& dir, double weight, int depth, bool isInternal) const
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
        if (material->refl > Const::EPS)
            ret += m_calcReflection(coll, material, weight, depth, isInternal);
        if (material->refr > Const::EPS)
            ret += m_calcRefraction(coll, material, weight, depth, isInternal);

        return ret.confine();
    }
    else
        return m_scene->getAmbientLightColor();
}
