#include "collision.h"
#include "common/bmp.h"
#include "common/const.h"
#include "light/light.h"
#include "light/pointlight.h"
#include "light/rectlight.h"
#include "object/object.h"
#include "object/plane.h"
#include "object/sphere.h"
#include "scene.h"

#include <algorithm>
#include <fstream>
#include <json/reader.h>
#include <json/writer.h>

Scene::Scene()
    : m_camera(new Camera()), m_ambient_color(Color())
{
    m_init();
}

Scene::Scene(const Json::Value& scene)
    : m_camera(new Camera(scene["camera"])), m_ambient_color(scene["ambient_color"])
{
    for (auto l : scene["lights"])
    {
        if (l["type"] == "PointLight")
            m_lights.push_back(new PointLight(l));
        else if (l["type"] == "RectLight")
            m_lights.push_back(new RectLight(l));
    }
    for (auto o : scene["objects"])
    {
        if (o["type"] == "Plane")
            m_objects.push_back(new Plane(o));
        else if (o["type"] == "Sphere")
            m_objects.push_back(new Sphere(o));
    }
    m_init();
}

Scene::~Scene()
{
    if (m_camera) delete m_camera;
    for (auto i : m_lights) delete i;
    for (auto i : m_objects) delete i;
    m_lights.clear();
    m_objects.clear();
}

Collision Scene::findNearestCollision(const Vector3& start, const Vector3& dir) const
{
    Collision ret;
    for (auto l : m_lights)
    {
        Collision coll = l->collide(start, dir);
        if (coll.isHit() && coll.dist + Const::EPS < ret.dist) ret = coll;
    }
    for (auto o : m_objects)
    {
        Collision coll = o->collide(start, dir);
        if (coll.isHit() && coll.dist + Const::EPS < ret.dist) ret = coll;
    }
    return ret;
}

Json::Value Scene::toJson() const
{
    Json::Value scene, lights, objects;
    for (auto l : m_lights) lights.append(l->toJson());
    for (auto o : m_objects) objects.append(o->toJson());
    scene["ambient_color"] = m_ambient_color.toJson();
    scene["lights"] = lights;
    scene["objects"] = objects;
    scene["camera"] = m_camera->toJson();
    return scene;
}

void Scene::save(const string& file) const
{
    std::ofstream fout(file.c_str());
    fout << this->toJson() << std::endl;
    fout.close();
}

Scene* Scene::loadFrom(const std::string& file)
{
    Json::Value scene;
    std::ifstream fin(file.c_str());
    if (!fin)
    {
        std::cout << "ERROR: No such scene file '" + file + "'" << std::endl;
        return nullptr;
    }
    fin >> scene;
    fin.close();
    return new Scene(scene);
}

void Scene::m_init()
{
    // 反射光比例小的物体优先，其次折射光
    sort(m_objects.begin(), m_objects.end(), [](const Object* A, const Object* B) {
        return A->getMaterial()->compare(B->getMaterial());
    });
}
