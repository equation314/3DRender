#ifndef SPHERE_H
#define SPHERE_H

#include "common/vector3.h"
#include "object/object.h"

class Sphere: public Object
{
public:
	Sphere(const Vector3& o, double r):
		Object(), m_o(o), m_r(r) {}
	
	virtual Collision collide(const Vector3& start, const Vector3& dir) const override;

private:
	Vector3 m_o; // 球心
	double m_r; // 半径
};

#endif // SPHERE_H