#include "common/bmp.h"
#include "common/const.h"
#include "scene/camera.h"

Camera::Camera()
    : m_eye(0, -1, -0.5), m_dir(0, 1, 0), m_up(0, 0, 1),
      m_w(800), m_h(600), m_fovy(45 * Const::PI / 180), m_dist(1)
{
    m_init();
}

Camera::Camera(const Vector3& eye, const Vector3& dir, const Vector3& up, int w, int h, double f, double d)
    : m_eye(eye), m_dir(dir), m_up(up),
      m_w(w), m_h(h), m_fovy(f * Const::PI / 180), m_dist(d)
{
    m_init();
}

Camera::Camera(const Json::Value& camera)
    : m_eye(camera["eye"]), m_dir(camera["dir"]), m_up(camera["up"]),
      m_w(camera["w"].asInt()), m_h(camera["h"].asInt()),
      m_fovy(camera["fovy"].asDouble() * Const::PI / 180), m_dist(camera["dist"].asDouble())
{
    m_init();
}

Camera::~Camera()
{
    delete m_film;
}

Vector3 Camera::emit(int x, int y) const
{
    return m_dir * m_dist + m_dw * (2.0 * x / m_w - 1) + m_dh * (2.0 * y / m_h - 1);
}

void Camera::setColor(int x, int y, const Color& color)
{
    m_film->setColor(x, y, color);
}

void Camera::print(const std::string& file) const
{
    m_film->save(file);
}

void Camera::m_init()
{
    m_dh = m_up * tan(m_fovy) * -m_dist;
    m_dw = (m_dh * m_dir).unitize() * tan(m_fovy) * m_dist * (1.0 * m_w / m_h);
    m_film = new Bmp(m_w, m_h);
}

Json::Value Camera::toJson() const
{
    Json::Value camera;
    camera["eye"] = m_eye.toJson();
    camera["dir"] = m_dir.toJson();
    camera["up"] = m_up.toJson();
    camera["w"] = m_w;
    camera["h"] = m_h;
    camera["fovy"] = m_fovy * 180 / Const::PI;
    camera["dist"] = m_dist;
    return camera;
}
