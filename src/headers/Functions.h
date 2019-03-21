#pragma once
#include "glm/glm.hpp"

void Start();
void Update();
void seasonalChange();
void timeChange();
glm::vec4 interpolateColor(glm::vec4 color1, glm::vec4 color2, float cosine);
float interpolateHeight(float val1, float val2, float sine);