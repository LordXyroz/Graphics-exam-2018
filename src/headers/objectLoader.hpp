#ifndef OBJECT_LOADER
#define OBJECT_LOADER

#include "Mesh.hpp"

enum FileExt
{
    OBJ,
};

std::vector <Mesh> loadObject(std::string fileName);
#endif // !OBJECT_LOADER

