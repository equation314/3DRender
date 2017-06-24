#include "object/rotationbody.h"

#include <cstring>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc <= 3 || strcmp(argv[2], "-o"))
        printf("Usage: ./modeling <OBJECT_FILE> -o <OUTPUT_FILE>\n");
    else
    {
        RotationBody* body = static_cast<RotationBody*>(Object::loadFrom(argv[1]));
        body->saveOBJ(argv[3], 100);
        delete body;
    }
    return 0;
}
