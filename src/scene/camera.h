#ifndef CAMERA_H
#define CAMERA_H

#include "common/color.h"
#include "common/vector3.h"

#include <string>
#include <vector>

class Bmp;

class Camera
{
public:
    Camera(const Vector3& eye, const Vector3& lookAt, const Vector3& up, int w, int h, double fovy, double aper = 0, double focal = 0);
    Camera(const Json::Value& camera);
    ~Camera();

    int getW() const { return m_w; }
    int getH() const { return m_h; }
    Vector3 getEye() const { return m_eye; }

    bool enableDOF() const;

    // 像素点对应的光线方向
    Ray emit(double x, double y) const;

    // 处理景深时的发射光线
    Ray dofEmit(double x, double y) const;

    Color getColor(int x, int y) const
    {
        return 0 <= x && x < m_w && 0 <= y && y < m_h ? m_color[x][y] : Color();
    }

    // 置像素点 (x, y) 的颜色为 color
    void setColor(int x, int y, const Color& color) { m_color[x][y] = color; }

    // 边缘检测
    std::vector<std::pair<int, int>> detectEdge() const;

    // 打印照片到 file 文件
    void print(const std::string& file) const;

    // 复制底片
    Bmp* copyFilm() const;

    // 重置底片为 BMP 文件
    void setFilm(const Bmp* film);

    Json::Value toJson() const;

private:
    Vector3 m_eye, m_look_at, m_dir, m_up; // 相机位置、相机视线上任意一点、视线方向、上方向
    int m_w, m_h;                          // 分辨率
    double m_fovy;                         // 相机视野张角
    double m_aperture, m_focal_len;        // 光圈大小，焦距(默认为 m_eye 和 m_look_at 的距离)
    Vector3 m_dw, m_dh;                    // 视平面坐标系方向
    Color** m_color;                       // 颜色缓存

    void m_init(); // 初始化
};

#endif // CAMERA_H
