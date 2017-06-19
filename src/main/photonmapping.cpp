#include "engine/photonmapper/photonmapper.h"

int main(int argc, char* argv[])
{
    if (argc <= 1)
        printf("Usage: ./photon_mapping <SCENE_FILE>\n");
    else
    {
        Engine* engine = new PhotonMapper();
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
