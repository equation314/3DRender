#include "object/object.h"
#include "object/plane.h"
#include "object/rotationbody.h"
#include "object/sphere.h"

#include <fstream>
#include <iostream>
#include <json/reader.h>
#include <json/writer.h>

Object::Object(const Material* m)
    : m_material(m == nullptr ? new Material() : m),
      m_can_delete_material(m == nullptr ? true : false)
{
}

Object::Object(const Json::Value& object)
    : m_material(object["material"].isNull() ? new Material() : new Material(object["material"])),
      m_can_delete_material(true)
{
}

Object::~Object()
{
    if (m_can_delete_material && m_material) delete m_material;
}

Json::Value Object::toJson() const
{
    Json::Value object;
    object["material"] = m_material->toJson();
    return object;
}

void Object::save(const std::string& file) const
{
    std::ofstream fout(file.c_str());
    fout << this->toJson() << std::endl;
    fout.close();
}

Object* Object::loadFromJson(const Json::Value& value)
{
    std::string type = value["type"].asString();
    if (type == "Plane")
        return new Plane(value);
    else if (type == "Sphere")
        return new Sphere(value);
    else if (type == "RotationBody")
        return new RotationBody(value);
    else
        return nullptr;
}

Object* Object::loadFrom(const std::string& file)
{
    Json::Value obj;
    std::ifstream fin(file.c_str());
    if (!fin)
    {
        std::cerr << "ERROR: No such scene file '" + file + "'" << std::endl;
        return nullptr;
    }
    fin >> obj;
    fin.close();

    return loadFromJson(obj);
}
