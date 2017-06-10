#include "common/bmp.h"
#include "common/const.h"
#include "scene/camera.h"

Camera::Camera(const Vector3& eye, const Vector3& lookAt, const Vector3& up, int w, int h, double fovy)
    : m_eye(eye), m_look_at(lookAt), m_dir((lookAt - eye).unitize()), m_up(up),
      m_w(w), m_h(h), m_fovy(fovy * Const::PI / 180)
{
    m_init();
}

Camera::Camera(const Json::Value& camera)
    : m_eye(camera["eye"]), m_look_at(camera["look_at"]), m_dir((m_look_at - m_eye).unitize()), m_up(camera["up"]),
      m_w(camera["w"].asInt()), m_h(camera["h"].asInt()),
      m_fovy(camera["fovy"].asDouble() * Const::PI / 180)
{
    m_init();
}

Camera::~Camera()
{
    delete m_film;
}

Vector3 Camera::emit(double x, double y) const
{
    return m_dir + m_dw * (2.0 * x / m_w - 1) + m_dh * (2.0 * y / m_h - 1);
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
    m_dw = (m_dir * m_up).unitize() * tan(m_fovy / 2) * (1.0 * m_w / m_h);
    m_dh = (m_dir * m_dw).unitize() * tan(m_fovy / 2);
    m_film = new Bmp(m_w, m_h);
}

Json::Value Camera::toJson() const
{
    Json::Value camera;
    camera["eye"] = m_eye.toJson();
    camera["look_at"] = m_look_at.toJson();
    camera["up"] = m_up.toJson();
    camera["w"] = m_w;
    camera["h"] = m_h;
    camera["fovy"] = m_fovy * 180 / Const::PI;
    return camera;
}
