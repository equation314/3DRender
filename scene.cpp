#include "scene.h"
#include "collision.h"
#include "common/const.h"
#include "light/light.h"
#include "light/pointlight.h"
#include "object/object.h"
#include "object/plane.h"
#include "object/sphere.h"

#include <algorithm>

Scene::Scene():
    m_camera(new Camera()), m_background_color(Color(0.2, 0.2, 0.2))
{
    m_lights.push_back(new PointLight(Color(1, 1, 1), Vector3(0, 0, 0)));

    Material* groundMaterial = new Material();
    Material* ballMaterial = new Material();
    Material* wallMaterial = new Material();

    groundMaterial->color = Color(0.8, 1, 1);
    groundMaterial->diff = 0;
    groundMaterial->spec = 0.8;
    groundMaterial->refl = 1.0;
    groundMaterial->refr = 0;

    ballMaterial->color = Color(1.0, 0, 0);
    ballMaterial->diff = 1.0;
    ballMaterial->spec = 1.0;
    ballMaterial->refl = 0;
    ballMaterial->refr = 0;

    wallMaterial->color = Color(1, 1, 1);
    wallMaterial->diff = 0.8;
    wallMaterial->spec = 0;
    wallMaterial->refl = 0;
    wallMaterial->refr = 0;

    Object* ground = new Plane(Vector3(0, 0, 1), 1);
    Object* ceiling = new Plane(Vector3(0, 0, -1), 1);
    Object* wall1 = new Plane(Vector3(0, -1, 0), 1);
    Object* wall2 = new Plane(Vector3(-1, 0, 0), 1);
    Object* wall3 = new Plane(Vector3(1, 0, 0), 1);
    Object* ball = new Sphere(Vector3(0, 2, 1), 1);

    ground->setMaterial(groundMaterial);
    ceiling->setMaterial(groundMaterial);
    wall1->setMaterial(wallMaterial);
    wall2->setMaterial(wallMaterial);
    wall3->setMaterial(wallMaterial);
    ball->setMaterial(ballMaterial);

    m_objects.push_back(ground);
    m_objects.push_back(ceiling);
    m_objects.push_back(wall1);
    m_objects.push_back(wall2);
    m_objects.push_back(wall3);
    // m_objects.push_back(ball);

    // 反射光比例小的物体优先，其次折射光
    sort(m_objects.begin(), m_objects.end(), [](Object* A, Object* B)
    {
        Material* a = A->getMaterial(), *b = B->getMaterial();
        return a->refl + EPS < b->refl || (abs(a->refl - b->refl) < EPS && a->refr + EPS < b->refr);
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
    for (auto l: m_lights)
    {
        Collision coll = l->collide(start, dir);
        if (coll.isHit() && coll.dist + EPS < ret.dist) ret = coll;
    }
    for (auto o: m_objects)
    {
        Collision coll = o->collide(start, dir);
        if (coll.isHit() && coll.dist + EPS < ret.dist) ret = coll;
    }
    return ret;
}
