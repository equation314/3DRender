#ifndef OBJECT_H
#define OBJECT_H

#include "collision.h"
#include "object/material.h"

class Plane;
class Sphere;

class Object
{
public:
	Object();
	virtual ~Object();

	Material* getMaterial() const { return m_material; }
	void setMaterial(Material* m) { m_material = m; }
	
	// 与视线相交
	virtual Collision collide(const Vector3& start, const Vector3& dir) const = 0;

protected:
	Material* m_material;
};

#endif // OBJECT_H