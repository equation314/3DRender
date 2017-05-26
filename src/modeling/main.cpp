#include "object/rotationbody.h"

int main()
{
    RotationBody* body = static_cast<RotationBody*>(Object::loadFrom("../scenes/glass.json"));
    body->saveOBJ("glass.obj", 100);
}
