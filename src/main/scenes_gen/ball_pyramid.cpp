#include "common/config.h"
#include "light/pointlight.h"
#include "light/rectlight.h"
#include "object/cylinder.h"
#include "object/plane.h"
#include "object/sphere.h"
#include "scene/camera.h"
#include "scene/scene.h"

int main()
{
    Config::depth_of_field_samples = 16;
    Config::anti_aliasing_samples = 2;
    Config::anti_aliasing_edge_detection_mode = 2;
    Config::ppm_photon_emitted_number = 10000;
    Config::ppm_initial_search_radius = 0.05;
    Config::ppm_photon_emitted_number = 100000;
    Config::ppm_iteration_depth = 1000;
    Config::thread_max_number = 4;
    Config::thread_max_number = 4;

    Scene* scene = new Scene(new Camera(Vector3(-2, -6, 2), Vector3(0.1, 0.1, 0), Vector3(0, 0, 1), 480, 300, 30, 0.05), Color(0.05, 0.05, 0.05));
    Material* glass = new Material(Color(1, 1, 1), 0, 0, 0.2, 0.8, 1.5, Color(0, 0, 0));

    Material* m_ground = new Material(Color(1, 1, 1), 0.9, 0.1);
    m_ground->setTexture(new Bmp("../textures/ball_pyramid/mosaic.bmp"));
    Material* m_wall = new Material(Color(1, 1, 1), 0.8, 0.1, 0.1);
    m_wall->setTexture(new Bmp("../textures/ball_pyramid/tile.bmp"));
    Material* m_ceil = new Material(Color(1, 1, 1), 0.9, 0.1);
    m_ceil->setTexture(new Bmp("../textures/ball_pyramid/ceil.bmp"));

    Plane* ground = new Plane(Vector3(0, 0, 1), 1, m_ground);
    ground->setTextureAxis(Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 0.8, 0));

    scene->addLight(new PointLight(Color(1, 1, 1), Vector3(2, 0, 3), 300));
    scene->addObject(ground);

    Plane* wall = new Plane(Vector3(0, -1, 0), 4, m_wall);
    wall->setTextureAxis(Vector3(2, 4, 0), Vector3(3, 0, 0), Vector3(0, 0, 6));
    scene->addObject(wall);

    wall = new Plane(Vector3(-1, 0, 0), 5, m_wall);
    wall->setTextureAxis(Vector3(5, -2, 0), Vector3(0, -3, 0), Vector3(0, 0, 6));
    scene->addObject(wall);

    scene->addObject(new Plane(Vector3(-1, 0, 0), 5, m_wall));
    scene->addObject(new Plane(Vector3(0, 0, -1), 7, m_ceil));
    scene->addObject(new Plane(Vector3(1, 0, 0), 7, m_wall));
    scene->addObject(new Plane(Vector3(0, 1, 0), 7, m_wall));
    scene->addObject(new Cylinder(Vector3(-0.5, 2.2, -1), 0.5, 1.5, glass));

    const double r = 0.3;
    const int n = 4;
    double x = 0, y = -r * sqrt(3), z = 0;

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n - k; i++)
            for (int j = 0; j < n - k; j++)
            {
                if (k == 1 && (i + j) & 1) continue;
                x = i * 2 * r + k * r - (n - 1) * r + 5 * r;
                y = j * 2 * r + k * r - 2 * r;
                z = sqrt(2) * r * k - 0.7;
                Material* colorGlass;
                if (k == 1 && i == 1 && j == 1)
                {
                    colorGlass = new Material(Color(1, 1, 1), 0.8, 0.1, 0.1);
                    colorGlass->setTexture(new Bmp("../textures/marble.bmp"));
                }
                else
                    colorGlass = new Material(Color(1, 1, 1), 0, 0, 0.2, 0.8, 1.5,
                                              Color(1 - 1.0 * k / n, 1 - 1.0 * (n - k - i - 1) / n, 1 - 1.0 * (n - k - j - 1) / n));

                scene->addObject(new Sphere(Vector3(x, y, z), r, colorGlass));
            }
    scene->save("ball_pyramid.json");

    return 0;
}
