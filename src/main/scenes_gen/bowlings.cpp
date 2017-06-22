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
    Config::depth_of_field_samples = 16;
    Config::anti_aliasing_samples = 2;
    Config::anti_aliasing_edge_detection_mode = 0;
    Config::ppm_photon_emitted_number = 10000;
    Config::ppm_initial_search_radius = 0.5;
    Config::ppm_photon_emitted_number = 1000000;
    Config::ppm_iteration_depth = 1000;
    Config::thread_max_number = 4;
    Config::thread_max_number = 4;

    Scene* scene = new Scene(new Camera(Vector3(-5, -6, 6), Vector3(-2, 17, 0), Vector3(0, 0, 1), 300, 480, 40, 0.25, 50), Color(0.0, 0.0, 0.0));
    Material* glass = new Material(Color(1, 1, 1), 0, 0, 0.2, 0.8, 1.5, Color(0.5, 0.5, 0));

    Material* m_ground = new Material(Color(1, 1, 1), 0.6, 0.1, 0.3);
    m_ground->setTexture(new Bmp("../textures/floor.bmp"));

    Plane* ground = new Plane(Vector3(0, 0, 1), 1, m_ground);
    ground->setTextureAxis(Vector3(0, 0, 0), Vector3(15, 0, 0), Vector3(0, 15, 0));

    scene->addLight(new PointLight(Color(1, 1, 1), Vector3(15, 5, 10), 13000));
    scene->addLight(new PointLight(Color(1, 1, 1), Vector3(-15, 5, 10), 13000));
    scene->addObject(ground);

    Sphere* sphere = new Sphere(Vector3(-1, 10, 1), 2, glass);
    scene->addObject(sphere);

    const double r = 3;
    const int n = 4;
    double x = 0, y = -r * sqrt(3);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n - i; j++)
        {
            y = 20 * r + -(i * 2 * r - (n - 1) * r + 5 * r);
            x = i * r + j * 2 * r - 2 * r;

            RotationBody* b = static_cast<RotationBody*>(Object::loadFrom("../scenes/bowlings/bowling.json"));
            b->setO(Vector3(x, y, -1));

            scene->addObject(b);
        }
    scene->save("bowlings.json");

    return 0;
}
