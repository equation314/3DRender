#include "engine/photonmapper/photonmapper.h"
#include "engine/ppm/ppm.h"
#include "engine/raytracer/raytracer.h"

#include <cstring>

void showHelp()
{
    printf("Usage: ./render [options] <SCENE_FILE>\n"
           "Options:\n"
           "  --help                    Display this information.\n"
           "  -e <engine>               Select the rendering engine. \n"
           "                            Available engine list:\n"
           "                              0: Ray tracing (default)\n"
           "                              1: Photon mapping\n"
           "                              2: Progressive photon papping\n");
    exit(0);
}

int main(int argc, char* argv[])
{
    int engineId = 0;
    string file = "";

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-e"))
        {
            if (i + 1 < argc && '0' <= argv[i + 1][0] && argv[i + 1][0] <= '2')
            {
                engineId = argv[i + 1][0] - '0';
                i++;
            }
            else
                showHelp();
        }
        else if (!strcmp(argv[i], "--help"))
            showHelp();
        else
            file = argv[i];
    }
    if (!file.length()) showHelp();

    Engine* engine;
    Scene* scene = Scene::loadFrom(file);
    switch (engineId)
    {
    case 0:
        engine = new RayTracer(scene);
        break;
    case 1:
        engine = new PhotonMapper(scene);
        break;
    case 2:
        engine = new PPM(scene);
        break;
    }

    engine->run("output.bmp");

    if (scene) delete scene;
    delete engine;

    return 0;
}
