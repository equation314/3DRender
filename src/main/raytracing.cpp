#include "engine/raytracer/raytracer.h"

int main(int argc, char* argv[])
{
    if (argc <= 1)
        printf("Usage: ./ray_tracing <SCENE_FILE>\n");
    else
    {
        Scene* scene = Scene::loadFrom(argv[1]);
        Engine* engine = new RayTracer(scene);
        engine->run("output.bmp");
        if (scene) delete scene;
        delete engine;
    }
    return 0;
}
