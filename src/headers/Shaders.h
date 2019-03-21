#pragma once
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"


class Shader
{
private:
    GLuint m_program_number;
public:
    Shader();
    Shader(const std::string& vertexString, const std::string& fragmentString);
    Shader(const std::string& name);

    void use();

    void setInt(const std::string& name, GLint value);
    void setInt(const std::string& name, GLint value, GLint value2);
    void setInt(const std::string& name, GLint value, GLint value2, GLint value3);
    void setInt(const std::string& name, GLint value, GLint value2, GLint value3, GLint value4);
    void setInt(const std::string& name, int value[36][28]);
    void setInt(const std::string& name, glm::ivec2 vec);
    void setInt(const std::string& name, glm::ivec3 vec);
    void setInt(const std::string& name, glm::ivec4 vec);

    void setFloat(const std::string& name, GLfloat value);
	void setFloat(const std::string& name, glm::vec2 value);
	void setFloat(const std::string& name, glm::vec3 value);
	void setFloat(const std::string& name, glm::vec4 value);

    void setMatrix(const std::string& name, glm::mat4 trans);
    void setMatrix(const std::string& name, glm::mat3 trans);
};