#include "collision.h"
#include "common/const.h"
#include "light/light.h"
#include "light/pointlight.h"
#include "object/object.h"
#include "object/plane.h"
#include "object/sphere.h"
#include "scene.h"

#include <algorithm>

Scene::Scene()
    : m_camera(new Camera()), m_background_color(Color(0.2, 0.2, 0.2))
{
    m_lights.push_back(new PointLight(Color(1, 1, 1), Vector3(0, 0, 0)));

    Material* groundMaterial = new Material(Color(0.8, 1, 1), 0, 0.8, 1.0, 0);
    Material* ballMaterial = new Material(Color(1.0, 0, 0), 0.8, 0.8, 0, 0);
    Material* wallMaterial = new Material(Color(1, 1, 1), 0.8, 0, 0, 0);

    Object* ground = new Plane(groundMaterial, Vector3(0, 0, 1), 1);
    Object* ceiling = new Plane(groundMaterial, Vector3(0, 0, -1), 1);
    Object* wall1 = new Plane(wallMaterial, Vector3(0, -1, 0), 1);
    Object* wall2 = new Plane(wallMaterial, Vector3(-1, 0, 0), 1);
    Object* wall3 = new Plane(wallMaterial, Vector3(1, 0, 0), 1);
    Object* ball = new Sphere(ballMaterial, Vector3(0, 0.5, -0.5), 1);

    m_objects.push_back(ground);
    m_objects.push_back(ceiling);
    m_objects.push_back(wall1);
    m_objects.push_back(wall2);
    m_objects.push_back(wall3);
    m_objects.push_back(ball);

    // 反射光比例小的物体优先，其次折射光
    sort(m_objects.begin(), m_objects.end(), [](const Object* A, const Object* B) {
        return A->getMaterial()->compare(B->getMaterial());
    });
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
