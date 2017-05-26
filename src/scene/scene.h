#ifndef SCENE_H
#define SCENE_H

#include <vector>

class Camera;
class Collision;
class Light;
class Object;

class Scene
{
public:
    Scene();
    Scene(const Json::Value& scene);
    ~Scene();

    Camera* getCamera() const { return m_camera; }
    Color getAmbientLightColor() const { return m_ambient_color; }
    Light* getLight(int i) const { return m_lights[i]; }
    Object* getObject(int i) const { return m_objects[i]; }

    void addLight(Light* light) { m_lights.push_back(light); }
    void addObject(Object* object) { m_objects.push_back(object); }

    std::vector<Light*>::const_iterator lightsBegin() const { return m_lights.begin(); }
    std::vector<Light*>::const_iterator lightsEnd() const { return m_lights.end(); }
    std::vector<Object*>::const_iterator objectsBegin() const { return m_objects.begin(); }
    std::vector<Object*>::const_iterator objectsEnd() const { return m_objects.end(); }

    // 寻找视线最先碰到的物体或光源
    Collision findNearestCollision(const Vector3& start, const Vector3& dir) const;

    // 保存为 JSON 格式
    Json::Value toJson() const;

    // 保存 JSON 到文件
    void save(const std::string& file) const;

    // 从文件导入场景
    static Scene* loadFrom(const std::string& file);

private:
    Camera* m_camera;      // 相机
    Color m_ambient_color; // 环境光

    std::vector<Light*> m_lights;
    std::vector<Object*> m_objects;

    static std::string m_scene_file_dir; // 场景文件路径

    void m_init();
};

#endif // SCENE_H
