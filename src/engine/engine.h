#ifndef ENGINE_H
#define ENGINE_H

#include "scene/scene.h"

class RayTracer;
class PhotonMapper;
class PPM;

class Engine
{
public:
    virtual ~Engine() {}

    virtual void run(const std::string& outFile) = 0;

protected:
    Engine(Scene* scene);

    Scene* m_scene;
    Camera* m_camera;
    int m_w, m_h;
};

#endif // ENGINE_H
