#include "engine/engine.h"
#include "scene/camera.h"

Engine::Engine(Scene* scene)
    : m_scene(scene), m_camera(nullptr), m_w(0), m_h(0)
{
    if (m_scene)
    {
        m_camera = m_scene->getCamera();
        m_w = m_camera->getW();
        m_h = m_camera->getH();
    }
}
