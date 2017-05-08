#include "raytracer.h"
#include "scene.h"

int main()
{
    RayTracer* engine = new RayTracer();
    Scene* scene = Scene::loadFrom("../scenes/scene1.json");
    engine->run(scene);
    return 0;
}
