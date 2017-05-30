#include "collision.h"

Collision::Collision()
    : dist(1e9)
{
}

Collision::Collision(const Vector3& start, const Vector3& dir, double t, const Vector3& n, const Object* obj)
    : object(obj), light(nullptr),
      ray_start(start), ray_dir(dir.unitize()), p(start + dir * t), n(n.unitize()),
      dist(t * dir.mod())
{
}

Collision::Collision(const Vector3& start, const Vector3& dir, double t, const Light* l)
    : object(nullptr), light(l),
      ray_start(start), ray_dir(dir.unitize()), p(start + dir * t),
      dist(t * dir.mod())
{
}
