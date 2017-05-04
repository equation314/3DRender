#include "camera.h"
#include "common/const.h"
#include "light/light.h"
#include "object/object.h"
#include "raytracer.h"

const double MIN_WEIGHT = 0.05;
const int MAX_DEPTH = 2;
const int SPEC_POWER = 50;

void RayTracer::run(Scene* scene)
{
    m_scene = scene;
    Camera* camera = scene->getCamera();
    int w = camera->getW(), h = camera->getH();

    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            Vector3 dir = camera->emit(i, j);
            Color color = m_rayTraceing(camera->getEye(), dir, 1, 1);
            camera->setColor(i, j, color);
        }

    camera->print("output.bmp");
}

Color RayTracer::m_calcLocalIllumination(const Collision& coll, Material* material) const
{
    Vector3 r = coll.ray_dir.reflect(coll.n);
    Color ret = material->color * m_scene->getBackgroundColor() * material->diff; // 环境光
    for (auto light = m_scene->lightsBegin(); light != m_scene->lightsEnd(); light++)
    {
        Vector3 l = ((*light)->getSource() - coll.o).unitize();
        if (material->diff > EPS) // 漫反射
            ret += material->color * (*light)->getColor() * (material->diff * l.dot(coll.n));
        if (material->spec > EPS) // 镜面反射
            ret += material->color * (*light)->getColor() * (material->spec * pow(l.dot(r), SPEC_POWER));
    }
    return ret;
}

Color RayTracer::m_calcReflection(const Collision& coll, Material* material, double weight, int depth) const
{
    Vector3 r = coll.ray_dir.reflect(coll.n);
    Color ret = m_rayTraceing(coll.o, r, weight * material->refl, depth + 1);
    return ret * material->color * material->refl;
}

Color RayTracer::m_calcRefraction(const Collision& coll, Material* material, double weight, int depth) const
{
    Vector3 r = coll.ray_dir.refract(coll.n, material->rindex);
    Color ret = m_rayTraceing(coll.o, r, weight * material->refr, depth + 1);
    return ret * material->color * material->refr;
}

Color RayTracer::m_rayTraceing(const Vector3& start, const Vector3& dir, double weight, int depth) const
{
    if (weight < MIN_WEIGHT) return Color();
    Collision coll = m_scene->findNearestCollision(start, dir);
    if (!coll.isHit())
        return m_scene->getBackgroundColor();
    // TODO 视线遇到光源
    // else if (coll.isLight())
    // 	return coll.getLight()->getColor();
    else if (depth <= MAX_DEPTH)
    {
        Color ret;
        const Object* obj = coll.object;
        if (obj->getMaterial()->diff > EPS || obj->getMaterial()->spec > EPS) ret += m_calcLocalIllumination(coll, obj->getMaterial());
        if (obj->getMaterial()->refl > EPS) ret += m_calcReflection(coll, obj->getMaterial(), weight, depth);
        if (obj->getMaterial()->refr > EPS) ret += m_calcRefraction(coll, obj->getMaterial(), weight, depth);

        ret = ret.confine();

        return ret;
    }
}
