#include "common/config.h"
#include "light/pointlight.h"
#include "light/rectlight.h"
#include "object/cylinder.h"
#include "object/plane.h"
#include "object/rotationbody.h"
#include "object/sphere.h"
#include "scene/camera.h"
#include "scene/scene.h"

int main()
{
    Scene* scene = Scene::loadFrom("../scenes/bowlings/cornell_house.json");

    Config::depth_of_field_samples = 4;
    Config::anti_aliasing_samples = 0;
    Config::anti_aliasing_edge_detection_mode = 0;
    Config::ppm_photon_emitted_number = 10000;
    Config::ppm_initial_search_radius = 0.02;
    Config::ppm_photon_emitted_number = 100000;
    Config::ppm_iteration_depth = 1000;
    Config::thread_max_number = 4;

    Material* glass = new Material(Color(0.7, 0.7, 0.7), 0.8, 0.2);

    RotationBody* l = static_cast<RotationBody*>(Object::loadFrom("../scenes/bowlings/lamp.json"));
    l->setO(Vector3(0, -0.5, 1.2));
    scene->addObject(l);
    scene->addObject(new Cylinder(Vector3(0, -0.5, 1.34), 0.005, 1, glass));

    l = static_cast<RotationBody*>(Object::loadFrom("../scenes/bowlings/lamp.json"));
    l->setO(Vector3(-0.5, 0.5, 1));
    scene->addObject(l);
    scene->addObject(new Cylinder(Vector3(-0.5, 0.5, 1.14), 0.005, 1, glass));

    l = static_cast<RotationBody*>(Object::loadFrom("../scenes/bowlings/lamp.json"));
    l->setO(Vector3(0.5, 0.5, 1));
    scene->addObject(l);
    scene->addObject(new Cylinder(Vector3(0.5, 0.5, 1.14), 0.005, 1, glass));

    l = static_cast<RotationBody*>(Object::loadFrom("../scenes/bowlings/lamp.json"));
    l->setO(Vector3(-0.5, -1.5, 1));
    scene->addObject(l);
    scene->addObject(new Cylinder(Vector3(-0.5, -1.5, 1.14), 0.005, 1, glass));

    l = static_cast<RotationBody*>(Object::loadFrom("../scenes/bowlings/lamp.json"));
    l->setO(Vector3(0.5, -1.5, 1));
    scene->addObject(l);
    scene->addObject(new Cylinder(Vector3(0.5, -1.5, 1.14), 0.005, 1, glass));

    const double r = 0.18;
    const int n = 4;
    double x = 0, y = -r * sqrt(3);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n - i; j++)
        {
            y = 1 - (i * 2 * r - (n - 1) * r + 5 * r);
            x = i * r + j * 2 * r - 3 * r;

            RotationBody* b = static_cast<RotationBody*>(Object::loadFrom("../scenes/bowlings/bowling.json"));
            b->setO(Vector3(x, y, 0));

            scene->addObject(b);
        }
    scene->save("bowlings.json");

    return 0;
}
