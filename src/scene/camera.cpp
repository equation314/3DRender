#include "common/bmp.h"
#include "common/config.h"
#include "common/const.h"
#include "common/ray.h"
#include "scene/camera.h"

#include <algorithm>

Camera::Camera(const Vector3& eye, const Vector3& lookAt, const Vector3& up, int w, int h, double fovy, double aper)
    : m_eye(eye), m_look_at(lookAt), m_dir((lookAt - eye).unitize()), m_up(up),
      m_w(w), m_h(h), m_fovy(fovy * Const::PI / 180), m_aperture(aper), m_focal_len((lookAt - eye).mod())
{
    m_init();
}

Camera::Camera(const Json::Value& camera)
    : m_eye(camera["eye"]), m_look_at(camera["look_at"]), m_dir((m_look_at - m_eye).unitize()), m_up(camera["up"]),
      m_w(camera["w"].asInt()), m_h(camera["h"].asInt()),
      m_fovy(camera["fovy"].asDouble() * Const::PI / 180),
      m_aperture(camera["aperture"].asDouble()),
      m_focal_len(camera["focal_len"].isDouble() ? camera["focal_len"].asDouble() : (m_look_at - m_eye).mod())
{
    m_init();
}

Camera::~Camera()
{
    for (int i = 0; i < m_w; i++) delete[] m_color[i];
    delete[] m_color;
}

bool Camera::enableDOF() const
{
    return m_aperture > 0 && Config::depth_of_field_samples > 0;
}

Ray Camera::emit(double x, double y) const
{
    return Ray(m_eye, m_dir + m_dw * (2.0 * x / m_w - 1) + m_dh * (2.0 * y / m_h - 1));
}

Ray Camera::dofEmit(double x, double y) const
{
    Vector3 focalPoint = m_eye + emit(x, y).dir.unitize() * m_focal_len;
    double dx, dy;
    for (;;)
    {
        dx = Const::randDouble() * 2 - 1;
        dy = Const::randDouble() * 2 - 1;
        if (dx * dx + dy * dy <= 1) break;
    }
    Vector3 start = m_eye + m_dw.unitize() * m_aperture * dx + m_dh.unitize() * m_aperture * dy;
    return Ray(start, focalPoint - start);
}

std::vector<pair<int, int>> Camera::detectEdge() const
{
    std::vector<pair<int, int>> list;
    for (int i = 0; i < m_w; i++)
        for (int j = 0; j < m_h; j++)
        {
            // Roberts cross operator
            Color gx = getColor(i, j) - getColor(i + 1, j + 1),
                  gy = getColor(i + 1, j) - getColor(i, j + 1);
            if (gx.mod2() + gy.mod2() > Config::anti_aliasing_edge_threshold)
            {
                list.push_back(make_pair(i, j));
                if (i < m_w - 1 && j < m_h - 1) list.push_back(make_pair(i + 1, j + 1));
                if (i < m_w - 1) list.push_back(make_pair(i + 1, j));
                if (j < m_h - 1) list.push_back(make_pair(i, j + 1));
            }
        }

    sort(list.begin(), list.end());
    auto iter = unique(list.begin(), list.end());
    list.erase(iter, list.end());
    return list;
}

void Camera::print(const std::string& file) const
{
    Bmp* film = new Bmp(m_w, m_h);
    for (int i = 0; i < m_w; i++)
        for (int j = 0; j < m_h; j++) film->setColor(i, j, m_color[i][j].confine());
    film->save(file);
    delete film;
}

Bmp* Camera::copyFilm() const
{
    Bmp* bmp = new Bmp(m_w, m_h);
    for (int i = 0; i < m_w; i++)
        for (int j = 0; j < m_h; j++) bmp->setColor(i, j, getColor(i, j));
    return bmp;
}

void Camera::setFilm(const Bmp* film)
{
    for (int i = 0; i < m_w; i++)
        for (int j = 0; j < m_h; j++) setColor(i, j, film->getColor(i, j));
}

void Camera::m_init()
{
    m_dw = (m_dir * m_up).unitize() * tan(m_fovy / 2) * (1.0 * m_w / m_h);
    m_dh = (m_dir * m_dw).unitize() * tan(m_fovy / 2);
    m_color = new Color*[m_w];
    for (int i = 0; i < m_w; i++) m_color[i] = new Color[m_h];
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
    camera["aperture"] = m_aperture;
    camera["focal_len"] = m_focal_len;
    if (m_aperture > 0)
    {
        camera["aperture"] = m_aperture;
        camera["focal_len"] = m_focal_len;
    }
    return camera;
}
