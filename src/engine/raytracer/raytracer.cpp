#include "common/config.h"
#include "common/const.h"
#include "engine/raytracer/raytracer.h"
#include "scene/camera.h"

#include <ctime>

void RayTracer::run(Scene* scene, const std::string& outFile)
{
    m_scene = scene;
    Camera* camera = scene->getCamera();
    int w = camera->getW(), h = camera->getH();

    cout << "Ray tracing..." << endl;
    clock_t lastRefreshTime = clock();
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            if (!j) cout << "column " << i << endl;
            Vector3 dir = camera->emit(i, j);
            Color color = m_rayTraceing(camera->getEye(), dir, 1, 1, false);
            camera->setColor(i, j, color);
        }
        if (Config::output_refresh_interval > 0 &&
            clock() - lastRefreshTime > Config::output_refresh_interval * CLOCKS_PER_SEC)
        {
            lastRefreshTime = clock();
            camera->print(outFile.c_str());
        }
    }

    camera->print(outFile.c_str());

    if (Config::anti_aliasing_samples)
    {
        cout << "Smoothing..." << endl;
        int samples = Config::anti_aliasing_samples;
        std::vector<pair<int, int>> list = camera->detectEdge();
        lastRefreshTime = clock();
        for (size_t t = 0; t < list.size(); t++)
        {
            if (!t || list[t].first != list[t - 1].first)
                cout << "column " << list[t].first << endl;

            int tot = 0;
            Color color;
            for (int i = 0; i < samples * 2; i++)
                for (int j = 0; j < samples * 2; j++)
                {
                    // 旋转网格采样
                    double a = atan(0.5);
                    double x = (i + 0.5) / samples - 1,
                           y = (j + 0.5) / samples - 1;
                    double dx = x * cos(a) - y * sin(a),
                           dy = x * sin(a) + y * cos(a);
                    if (dx > -0.5 && dx < 0.5 && dy > -0.5 && dy < 0.5)
                    {
                        Vector3 dir = camera->emit(list[t].first + dx, list[t].second + dy);
                        color += m_rayTraceing(camera->getEye(), dir, 1, 1, false);
                        tot++;
                    }
                }
            camera->setColor(list[t].first, list[t].second, color / tot);

            if (Config::output_refresh_interval > 0 &&
                clock() - lastRefreshTime > Config::output_refresh_interval * CLOCKS_PER_SEC)
            {
                lastRefreshTime = clock();
                camera->print(outFile.c_str());
            }
        }

        camera->print(outFile.c_str());
    }
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
            ret += color * (*light)->getColor() * (material->spec * pow(l.dot(r), Config::hightlight_exponent));
    }
    return ret;
}

Color RayTracer::m_rayTraceing(const Vector3& start, const Vector3& dir, double weight, int depth, bool isInternal) const
{
    if (weight < Config::raytracing_min_weight) return m_scene->getAmbientLightColor();
    Collision coll = m_scene->findNearestCollision(start, dir);
    if (!coll.isHit())
        return m_scene->getAmbientLightColor();
    else if (coll.atLight())
        return coll.light->getColor();
    else if (depth <= Config::raytracing_max_depth)
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
                double kl = material->refl, kr = material->refr;
                if (Config::enable_fresnel) // Fresnel equations
                {
                    double cosI = -coll.ray_dir.dot(coll.n), cosT = sqrt(1 - (1 - cosI * cosI) / n / n);
                    double r1 = (cosI * n - cosT) / (cosI * n + cosT),
                           r2 = (cosI - cosT * n) / (cosI + cosT * n);
                    kl = (r1 * r1 + r2 * r2) / 2, kr = 1 - kl;
                }

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
