#include "scene.h"
#include "raytracer.h"

int main()
{
	RayTracer* engine = new RayTracer();
	Scene* scene = new Scene();
	engine->run(scene);
	return 0;
}
