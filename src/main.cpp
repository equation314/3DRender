#include "raytracer.h"
#include "scene.h"

int main()
{
    RayTracer* engine = new RayTracer();
    Scene* scene = new Scene();
    engine->run(scene);
    return 0;
}
