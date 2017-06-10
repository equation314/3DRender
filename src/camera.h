#ifndef CAMERA_H
#define CAMERA_H

#include <string>

#include "common/color.h"
#include "common/vector3.h"

class Bmp;

class Camera
{
public:
    Camera(const Vector3& eye, const Vector3& lookAt, const Vector3& up, int w, int h, double fovy);
    Camera(const Json::Value& camera);
    ~Camera();

    int getW() const { return m_w; }
    int getH() const { return m_h; }
    Vector3 getEye() const { return m_eye; }

    // 像素点对应的光线方向
    Vector3 emit(double x, double y) const;

    // 置像素点 (x, y) 的颜色为 color
    void setColor(int x, int y, const Color& color);

    // 打印照片到 file 文件
    void print(const std::string& file) const;

    Json::Value toJson() const;

private:
    Vector3 m_eye, m_look_at, m_dir, m_up; // 相机位置、相机视线上任意一点、视线方向、上方向
    int m_w, m_h;                          // 分辨率
    double m_fovy;                         // 相机视野张角
    Vector3 m_dw, m_dh;                    // 视平面坐标系方向
    Bmp* m_film;                           // 底片

    void m_init(); // 初始化
};

#endif // CAMERA_H
