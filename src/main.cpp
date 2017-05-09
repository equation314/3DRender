#include "raytracer.h"
#include "scene.h"

int main()
{
    RayTracer* engine = new RayTracer();
    Scene* scene = Scene::loadFrom("../scenes/scene1.json");
    if (scene)
    {
        engine->run(scene);
        delete scene;
    }

    delete engine;
    return 0;
}
