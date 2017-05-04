#ifndef LIGHT_H
#define LIGHT_H

#include "common/color.h"
#include "collision.h"

class PointLight;

class Light
{
public:
	Light(const Color& c): m_color(c) {}
	virtual ~Light() {}

	Color getColor() const { return m_color; }

	// 发光点
	virtual Vector3 getSource() const = 0;

	// 与视线相交
	virtual Collision collide(const Vector3& start, const Vector3& dir) const = 0;

protected:
	Color m_color;
};

#endif // LIGHT_H
