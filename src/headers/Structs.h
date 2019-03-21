#pragma once
#include <vector>

#include "Shaders.h"
#include "Texture.h"

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>


// Container for pointers to shader and texture, 
// Different materials may contain different combinations of shaders/textures
struct Material
{
    Shader* shader;
    Texture* texture;
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float specularity = 64;
};

// Container with functions for position and rotation
struct Transform
{
    glm::vec3 position;
    glm::mat4 rotation;
	glm::vec3 scale;
    float speed;
    void rotate(float angle, glm::vec3 direction, double delta_time)
    {
        rotation = glm::rotate(rotation, glm::radians(angle) * (float) delta_time, direction);
    }
    void translate(glm::vec3 direction, double delta_time)
    {
        position += (direction * (float)delta_time * speed);
    }
};

// Container for different buffer objects
struct Buffer
{
    GLuint VAO;
    GLuint VBO[3];
    GLuint IBO;
};


struct Light
{
	Transform transform;
	glm::vec3 color = glm::vec3(1.0f);
};