#include "object/rotationbody.h"

int main(int argc, char* argv[])
{
    if (argc <= 1)
        printf("Usage: ./modeling <OBJECT_FILE>\n");
    else
    {
        RotationBody* body = static_cast<RotationBody*>(Object::loadFrom(argv[1]));
        body->saveOBJ("output.obj", 100);
        delete body;
    }
    return 0;
}
