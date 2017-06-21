#include "common/config.h"
#include "common/const.h"
#include "engine/raytracer/raytracer.h"
#include "scene/camera.h"

#include <ctime>

void RayTracer::run(const std::string& outFile)
{
    if (!m_scene) return;
    int w = m_camera->getW(), h = m_camera->getH();

    cout << "Ray tracing..." << endl;
    clock_t lastRefreshTime = clock();
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            if (!j) cout << "column " << i << endl;
            m_camera->setColor(i, j, m_dofSamplingColor(i, j));
        }
        if (Config::output_refresh_interval > 0 &&
            clock() - lastRefreshTime > Config::output_refresh_interval * CLOCKS_PER_SEC)
        {
            lastRefreshTime = clock();
            m_camera->print(outFile.c_str());
        }
    }

    m_camera->print(outFile.c_str());

    if (Config::anti_aliasing_samples)
    {
        cout << "Smoothing..." << endl;
        std::vector<pair<int, int>> list = m_camera->detectEdge();
        lastRefreshTime = clock();
        for (size_t t = 0; t < list.size(); t++)
        {
            if (!t || list[t].first != list[t - 1].first)
                cout << "column " << list[t].first << endl;

            m_camera->setColor(list[t].first, list[t].second, m_samplingColor(list[t].first, list[t].second));

            if (Config::output_refresh_interval > 0 &&
                clock() - lastRefreshTime > Config::output_refresh_interval * CLOCKS_PER_SEC)
            {
                lastRefreshTime = clock();
                m_camera->print(outFile.c_str());
            }
        }

        m_camera->print(outFile.c_str());
    }
}

Color RayTracer::m_dofSamplingColor(int ox, int oy, double factor) const
{
    if (!Config::depth_of_field_samples)
        return m_rayTracing(m_camera->emit(ox, oy), Color(1, 1, 1) * factor, 1, 1, false);

    Color color;
    for (int i = 0; i < Config::depth_of_field_samples; i++)
        color += m_rayTracing(m_camera->dofEmit(ox, oy), Color(1, 1, 1) * factor / Config::depth_of_field_samples, 1, 1, false);
    return color;
}

Color RayTracer::m_samplingColor(int ox, int oy) const
{
    if (!Config::anti_aliasing_samples)
        return m_dofSamplingColor(ox, oy);

    std::vector<pair<double, double>> points;
    int samples = Config::anti_aliasing_samples;
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
                points.push_back(make_pair(ox + dx, oy + dy));
        }
    Color color;
    for (auto p : points)
        color += m_dofSamplingColor(p.first, p.second, 1.0 / points.size());
    return color;
}

Color RayTracer::m_calcLocalIllumination(const Collision& coll, const Material* material, const Color& factor) const
{
    Color color = material->color * coll.object->getTextureColor(coll);
    Color ret = color * m_scene->getAmbientLightColor() * material->diff; // 环境光
    for (auto light = m_scene->lightsBegin(); light != m_scene->lightsEnd(); light++)
    {
        Vector3 l = ((*light)->getSource() - coll.p).unitize();
        if (l.dot(coll.n) < Const::EPS) continue;

        double shade = (*light)->getShadowRatio(m_scene, coll.p);
        if (shade > Const::EPS)
            ret += color * (*light)->getColor() * material->BRDF(l, coll.n, coll.ray.dir) * shade;
    }
    return ret * factor;
}

Color RayTracer::m_rayTracing(const Ray& ray, const Color& factor, double weight, int depth, bool isInternal) const
{
    if (weight < Config::raytracing_min_weight || depth > Config::raytracing_max_depth)
        return Color();

    Collision coll = m_scene->findNearestCollision(ray);
    if (!coll.isHit())
        return m_scene->getAmbientLightColor() * factor;
    else if (coll.atLight())
        return coll.light->getColor() * factor;
    else
    {
        Color ret, absorb(1, 1, 1);
        const Object* obj = coll.object;
        const Material* material = obj->getMaterial();

        // 透明材质的颜色过滤
        if (isInternal) absorb = (material->absorb_color * -coll.dist).exp();

        if (material->diff > Const::EPS || material->spec > Const::EPS)
            ret += m_calcLocalIllumination(coll, material, factor * absorb);
        if (material->refl > Const::EPS || material->refr > Const::EPS)
        {
            double n = material->rindex;
            if (isInternal) n = 1 / n;
            Vector3 refl = coll.ray.dir.reflect(coll.n);
            Vector3 refr = coll.ray.dir.refract(coll.n, n);
            if (material->refr < Const::EPS) // 全镜面反射
                ret += m_rayTracing(Ray(coll.p, refl),
                                    factor * absorb * (material->color * material->refl), weight * material->refl,
                                    depth + 1, isInternal);
            else if (refr.mod2() < Const::EPS) // 全反射
            {
                double k = material->refl + material->refr;
                ret += m_rayTracing(Ray(coll.p, refl),
                                    factor * absorb * (material->color * k), weight * k,
                                    depth + 1, isInternal);
            }
            else if (material->refl < Const::EPS) // 全透射
                ret += m_rayTracing(Ray(coll.p, refr),
                                    factor * absorb * material->refr, weight * material->refr,
                                    depth + 1, !isInternal);
            else
            {
                double kl = material->refl, kr = material->refr;
                if (Config::enable_fresnel) // Fresnel equations
                {
                    double cosI = -coll.ray.dir.dot(coll.n), cosT = sqrt(1 - (1 - cosI * cosI) / n / n);
                    double r1 = (cosI * n - cosT) / (cosI * n + cosT),
                           r2 = (cosI - cosT * n) / (cosI + cosT * n);
                    kl = (r1 * r1 + r2 * r2) / 2, kr = 1 - kl;
                }

                if (kl > Const::EPS) ret += m_rayTracing(Ray(coll.p, refl),
                                                         factor * absorb * (material->color * kl), weight * kl,
                                                         depth + 1, isInternal);
                if (kr > Const::EPS) ret += m_rayTracing(Ray(coll.p, refr),
                                                         factor * absorb * kr, weight * kr,
                                                         depth + 1, !isInternal);
            }
        }

        return ret;
    }
}
