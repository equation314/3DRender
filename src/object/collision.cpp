#include "collision.h"

Collision::Collision()
    : dist(1e9)
{
}

Collision::Collision(const Ray& ray, double t, const Vector3& n, const Object* obj, bool in)
    : object(obj), light(nullptr),
      ray(ray.unitize()), p(ray.get(t)), n(n.unitize()),
      dist(t * ray.dir.mod()), is_internal(in)
{
}

Collision::Collision(const Ray& ray, double t, double u, double v, const Vector3& n, const Object* obj, bool in)
    : object(obj), light(nullptr),
      ray(ray.unitize()), p(ray.get(t)), n(n.unitize()),
      dist(t * ray.dir.mod()), u(u), v(v), is_internal(in)
{
}

Collision::Collision(const Ray& ray, double t, const Light* l)
    : object(nullptr), light(l),
      ray(ray.unitize()), p(ray.get(t)),
      dist(t * ray.dir.mod()), is_internal(false)
{
}
