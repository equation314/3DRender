#include "raytracer.h"
#include "scene.h"

int main()
{
    RayTracer* engine = new RayTracer();
    Scene* scene = new Scene();
    scene->save("scene.json");
    engine->run(scene);
    return 0;
}
