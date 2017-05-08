#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "light/light.h"
#include "object/object.h"

#include <vector>

class Collision;

class Scene
{
public:
    Scene();
    ~Scene();

    Camera* getCamera() const { return m_camera; }
    Color getAmbientLightColor() const { return m_ambient_color; }

    std::vector<Light*>::const_iterator lightsBegin() const { return m_lights.begin(); }
    std::vector<Light*>::const_iterator lightsEnd() const { return m_lights.end(); }
    std::vector<Object*>::const_iterator objectsBegin() const { return m_objects.begin(); }
    std::vector<Object*>::const_iterator objectsEnd() const { return m_objects.end(); }

    // 寻找视线最先碰到的物体或光源
    Collision findNearestCollision(const Vector3& start, const Vector3& dir) const;

    // 保存为 JSON 格式
    Json::Value toJson() const;

    // 导入场景
    void load(const std::string& file);

    // 保存 JSON 到文件
    void save(const std::string& file) const;

private:
    Camera* m_camera;      // 相机
    Color m_ambient_color; // 环境光

    std::vector<Light*> m_lights;
    std::vector<Object*> m_objects;
};

#endif // SCENE_H
