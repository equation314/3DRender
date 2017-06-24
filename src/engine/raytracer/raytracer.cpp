#include "common/config.h"
#include "common/const.h"
#include "engine/raytracer/raytracer.h"
#include "scene/camera.h"

#include <algorithm>
#include <ctime>

RayTracer::RayTracer(Scene* scene)
    : Engine(scene)
{
    if (scene)
    {
        m_hash = new uint64*[m_w];
        m_is_edge = new bool*[m_w];
        for (int i = 0; i < m_w; i++)
        {
            m_hash[i] = new uint64[m_h];
            m_is_edge[i] = new bool[m_h];
        }
    }
}

RayTracer::~RayTracer()
{
    if (m_scene)
    {
        for (int i = 0; i < m_w; i++) delete[] m_hash[i];
        delete[] m_hash;
    }
}

void RayTracer::run(const std::string& outFile)
{
    if (!m_scene) return;

    cout << "Ray tracing..." << endl;
    clock_t lastRefreshTime = clock();
    for (int i = 0; i < m_w; i++)
    {
        for (int j = 0; j < m_h; j++)
        {
            if (!j) cout << "column " << i << endl;
            m_pixel_x = i, m_pixel_y = j;
            m_hash[i][j] = 0;
            m_camera->setColor(i, j, m_DOFSamplingColor(i, j));
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

        std::vector<pair<int, int>> list;
        switch (Config::anti_aliasing_edge_detection_mode)
        {
        case 0:
            for (int i = 0; i < m_w; i++)
                for (int j = 0; j < m_h; j++)
                {
                    m_is_edge[i][j] = false;
                    if (i < m_w - 1 && j < m_h - 1)
                        if (m_hash[i][j] != m_hash[i + 1][j + 1] || m_hash[i + 1][j] != m_hash[i][j + 1])
                        {
                            list.push_back(make_pair(i, j));
                            list.push_back(make_pair(i + 1, j + 1));
                            list.push_back(make_pair(i + 1, j));
                            list.push_back(make_pair(i, j + 1));
                        }
                }
            sort(list.begin(), list.end());
            list.erase(unique(list.begin(), list.end()), list.end());
            break;
        case 1:
            list = m_camera->detectEdge();
            break;
        default:
            for (int i = 0; i < m_w; i++)
                for (int j = 0; j < m_h; j++) list.push_back(make_pair(i, j));
            break;
        }

        lastRefreshTime = clock();
        for (size_t t = 0; t < list.size(); t++)
        {
            if (!t || list[t].first != list[t - 1].first)
                cout << "column " << list[t].first << endl;

            m_is_edge[list[t].first][list[t].second] = true;
            m_camera->setColor(list[t].first, list[t].second, m_AASamplingColor(list[t].first, list[t].second));

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

Color RayTracer::m_DOFSamplingColor(double ox, double oy, double factor) const
{
    if (!m_camera->enableDOF())
        return m_rayTracing(m_camera->emit(ox, oy), Color(1, 1, 1) * factor, 1, 1);

    Color color;
    for (int i = 0; i < Config::depth_of_field_samples; i++)
        color += m_rayTracing(m_camera->dofEmit(ox, oy), Color(1, 1, 1) * factor / Config::depth_of_field_samples, 1, 1);
    return color;
}

Color RayTracer::m_AASamplingColor(int ox, int oy)
{
    m_pixel_x = ox, m_pixel_y = oy;
    if (!Config::anti_aliasing_samples)
        return m_DOFSamplingColor(ox, oy);

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
        color += m_DOFSamplingColor(p.first, p.second, 1.0 / points.size());
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

Color RayTracer::m_rayTracing(const Ray& ray, const Color& factor, double weight, int depth) const
{
    if (weight < Config::raytracing_min_weight || depth > Config::raytracing_max_depth)
        return Color();

    Collision coll = m_scene->findNearestCollision(ray);
    if (!coll.isHit())
        return m_scene->getAmbientLightColor() * factor;
    else if (coll.atLight())
    {
        (m_hash[m_pixel_x][m_pixel_y] *= Const::HASH_BASE) += coll.object_identifier;
        return coll.light->getColor() * factor;
    }
    else
    {
        Color ret, absorb(1, 1, 1);
        const Object* obj = coll.object;
        const Material* material = obj->getMaterial();
        (m_hash[m_pixel_x][m_pixel_y] *= Const::HASH_BASE) += coll.object_identifier;

        // 透明材质的颜色过滤
        if (coll.is_internal) absorb = (material->absorb_color * -coll.dist).exp();

        if (material->diff > Const::EPS || material->spec > Const::EPS)
            ret += m_calcLocalIllumination(coll, material, factor * absorb);
        if (material->refl > Const::EPS || material->refr > Const::EPS)
        {
            double n = material->rindex;
            if (coll.is_internal) n = 1 / n;
            Vector3 refl = coll.ray.dir.reflect(coll.n);
            Vector3 refr = coll.ray.dir.refract(coll.n, n);
            if (material->refr < Const::EPS) // 全镜面反射
                ret += m_rayTracing(Ray(coll.p, refl), factor * absorb * (material->color * material->refl),
                                    weight * material->refl, depth + 1);
            else if (refr.mod2() < Const::EPS) // 全反射
            {
                double k = material->refl + material->refr;
                ret += m_rayTracing(Ray(coll.p, refl),
                                    factor * absorb * (material->color * k),
                                    weight * k, depth + 1);
            }
            else if (material->refl < Const::EPS) // 全透射
                ret += m_rayTracing(Ray(coll.p, refr), factor * absorb * material->refr,
                                    weight * material->refr, depth + 1);
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

                if (kl > Const::EPS) ret += m_rayTracing(Ray(coll.p, refl), factor * absorb * (material->color * kl),
                                                         weight * kl, depth + 1);
                if (kr > Const::EPS) ret += m_rayTracing(Ray(coll.p, refr), factor * absorb * kr,
                                                         weight * kr, depth + 1);
            }
        }

        return ret;
    }
}
