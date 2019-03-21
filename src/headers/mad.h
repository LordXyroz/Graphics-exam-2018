#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

namespace mad
{
    // Function for reading files, returns true if successful
    inline bool readStringFromFile(const char* filepath, std::string& stringData)
    {
        std::ifstream stringFile;
        stringFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            stringFile.open(filepath);
            std::stringstream shaderStream;
            shaderStream << stringFile.rdbuf();
            stringData = shaderStream.str();
        }
        catch (const std::ifstream::failure& e)
        {
            std::cout << "ERROR::FILE::READ::" << filepath << "...\n" << e.what() << std::endl;
            return false;
        }
        return true;
    }



}
