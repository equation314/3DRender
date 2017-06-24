#include "engine/photonmapper/photonmapper.h"
#include "engine/ppm/ppm.h"
#include "engine/raytracer/raytracer.h"

#include <cstdlib>
#include <cstring>

void showHelp()
{
    printf("Usage: ./render [options] <SCENE_FILE> -o <OUTPUT_FILE>\n"
           "Options:\n"
           "  --help                    Display this information.\n"
           "  -e <engine>               Select the rendering engine. Available engine list:\n"
           "                              0: Ray tracing (default)\n"
           "                              1: Photon mapping\n"
           "                              2: Progressive photon papping\n");
    exit(0);
}

int main(int argc, char* argv[])
{
    int engineId = 0;
    std::string scenefile = "", outFile = "";

    if (argc > 1)
        scenefile = argv[1];
    for (int i = 2; i < argc; i++)
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
        else if (!strcmp(argv[i], "-o"))
        {
            if (i + 1 < argc)
            {
                outFile = argv[i + 1];
                i++;
            }
            else
                showHelp();
        }
        else if (!strcmp(argv[i], "--help"))
            showHelp();
    }
    if (!scenefile.length() || !outFile.length()) showHelp();

    srand(time(0));

    Engine* engine;
    Scene* scene = Scene::loadFrom(scenefile);
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

    engine->run(outFile);

    if (scene) delete scene;
    delete engine;

    return 0;
}
