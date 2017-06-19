#include "engine/raytracer.h"

int main(int argc, char* argv[])
{
    if (argc <= 1)
        printf("Usage: ./ray_tracing <SCENE_FILE>\n");
    else
    {
        Engine* engine = new RayTracer();
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
