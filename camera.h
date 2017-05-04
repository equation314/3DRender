#ifndef CAMERA_H
#define CAMERA_H

#include <string>

#include "common/color.h"
#include "common/vector3.h"

class Bmp;

class Camera
{
public:
	Camera();
	Camera(const Vector3& eye, const Vector3& dir, const Vector3& up, int w, int h, double f, double d);
	~Camera();

	int getW() const { return m_w; }
	int getH() const { return m_h; }
	Vector3 getEye() const { return m_eye; }

	// 像素点对应的光线方向
	Vector3 emit(int x, int y) const;

	// 置像素点 (x, y) 的颜色为 color
	void setColor(int x, int y, const Color& color);

	// 打印照片到 file 文件
	void print(const std::string& file) const;

private:
	Vector3 m_eye, m_dir, m_up; // 相机位置、视线方向、上方向，up 需与 dir 垂直
	int m_w, m_h; // 分辨率
	double m_fovy, m_dist; // 相机上方视野张角，视平面到相机位置的距离
	Vector3 m_dw, m_dh; // 视平面坐标系方向
	Bmp* m_film; // 底片

	void m_init(); // 初始化
};

#endif // CAMERA_H
