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

Scene::Scene()
    : m_camera(new Camera()), m_ambient_color(Color(0.1, 0.1, 0.1))
{
    m_lights.push_back(new RectLight(Color(1, 1, 1), Vector3(0, -0.8, 0.5), Vector3(0.3, 0, 0), Vector3(0, 0.3, 0)));
    // m_lights.push_back(new PointLight(Color(1, 1, 1), Vector3(0, -1, 0.9)));

    Material* groundMaterial = new Material(Color(1, 1, 1), 0.8, 0.2, 0.2);
    Material* mirrorMaterial = new Material(Color(0.9, 0.9, 0.9), 0.1, 0.8, 0.9);
    Material* redWall = new Material(Color(1, 0, 0), 0.9, 0.2, 0.1);
    Material* blueWall = new Material(Color(0, 0, 1), 0.9, 0.2, 0.1);
    Material* writeWall = new Material(Color(1, 1, 1), 0.8, 0.2, 0);
    Material* blueGlass = new Material(Color(0.3, 0.3, 1), 0, 0.8, 0.4, 0.6, 1.5, Color(1, 1, 0));
    Material* writeGlass = new Material(Color(1, 1, 1), 0.8, 0.5, 0);

    writeGlass->setTexture(new Bmp("../textures/earth.bmp"));
    writeWall->setTexture(new Bmp("../textures/board.bmp"));
    groundMaterial->setTexture(new Bmp("../textures/floor.bmp"));
    blueWall->setTexture([](double u, double v) {
        return (int(u * 8) + int(v * 8)) & 1 ? Color(1, 1, 1) : Color(0.5, 0.5, 0.5);
    });
    redWall->setTexture([](double u, double v) {
        return (int(u * 8) + int(v * 8)) & 1 ? Color(1, 1, 1) : Color(0.5, 0.5, 0.5);
    });

    Object* ground = new Plane(groundMaterial, Vector3(0, 0, 1), 1);
    Object* ceiling = new Plane(writeWall, Vector3(0, 0, -1), 1);
    Object* wall1 = new Plane(writeWall, Vector3(0, -1, 0), 1);
    Object* wall2 = new Plane(blueWall, Vector3(-1, 0, 0), 1);
    Object* wall3 = new Plane(redWall, Vector3(1, 0, 0), 1);
    Object* wall4 = new Plane(writeWall, Vector3(0, 1, 0), 1);
    Object* ball1 = new Sphere(writeGlass, Vector3(0.5, 0.2, -0.6), 0.4);
    Object* ball2 = new Sphere(blueGlass, Vector3(-0.5, 0.2, -0.6), 0.4);
    ((Plane*) wall1)->setTextureAxis(Vector3(-1, 0, -1), Vector3(0, 0, 1), Vector3(2, 0, 0));

    m_objects.push_back(ground);
    m_objects.push_back(ceiling);
    m_objects.push_back(wall1);
    m_objects.push_back(wall2);
    m_objects.push_back(wall3);
    m_objects.push_back(wall4);
    m_objects.push_back(ball1);
    m_objects.push_back(ball2);

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
