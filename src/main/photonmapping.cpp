#include "engine/photonmapper/photonmapper.h"

int main(int argc, char* argv[])
{
    if (argc <= 1)
        printf("Usage: ./photon_mapping <SCENE_FILE>\n");
    else
    {
        Scene* scene = Scene::loadFrom(argv[1]);
        Engine* engine = new PhotonMapper(scene);
        engine->run("output.bmp");
        if (scene) delete scene;
        delete engine;
    }
    return 0;
}
