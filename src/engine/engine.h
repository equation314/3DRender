#ifndef ENGINE_H
#define ENGINE_H

#include "scene/scene.h"

class RayTracer;
class PhotonMapper;

class Engine
{
public:
    virtual ~Engine() {}

    virtual void run(Scene* scene, const std::string& outFile) = 0;

protected:
    Engine();

    Scene* m_scene;
};

#endif // ENGINE_H
