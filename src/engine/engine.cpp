#include "engine/engine.h"

Engine::Engine(Scene* scene)
    : m_scene(scene), m_camera(nullptr)
{
    if (m_scene) m_camera = m_scene->getCamera();
}
