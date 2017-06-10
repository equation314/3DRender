#include "raytracing/raytracer.h"
#include "scene/scene.h"

int main(int argc, char* argv[])
{
    if (argc <= 1)
        printf("Usage: ./raytracing <SCENE_FILE>\n");
    else
    {
        RayTracer* engine = new RayTracer();
        Scene* scene = Scene::loadFrom(argv[1]);
        if (scene)
        {
            engine->run(scene, "output.bmp");
            delete scene;
        }
        delete engine;
    }
    return 0;
}
