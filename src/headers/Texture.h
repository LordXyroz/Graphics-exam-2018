#pragma once
#include <string>
#include "src/headers/Mesh.hpp"

#include <GL/glew.h>
#include <SDL2_ttf/include/SDL_ttf.h>

//#include <SDL2/SDL_surface.h>


class Texture
{
private:
    GLuint m_texture_number;

    SDL_Surface* message;
public:
    Texture();
    Texture(const char* path);

    int createTextureFromText(TTF_Font* font, std::string text, SDL_Color color);

    GLuint getNo();
};
