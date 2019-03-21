#pragma once
#include "src/headers/GameObject.h"
#include "src/headers/Enums.h"
#include "glm/glm.hpp"

#include "SDL2/include/SDL.h"


class TextBox : public GameObject
{
private:
    TTF_Font* font;
    glm::mat4 projection;

public:
    TextBox();
    void initBox();

    void draw(std::string text, SDL_Color color, glm::vec3 pos, Allignment allign);
};