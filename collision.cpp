#include "collision.h"

Collision::Collision():
	dist(1e9)
{

}

Collision::Collision(const Vector3& start, const Vector3& dir, double t, const Vector3& n, const Object* obj):
	object(obj),
	ray_start(start), ray_dir(dir.unitize()), o(start + dir * t), n(n.unitize()),
	dist(t * dir.mod())
{

}
