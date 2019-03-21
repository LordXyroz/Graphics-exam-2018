#include "src/headers/Shaders.h"
#include "src/headers/logger.h"
#include "src/headers/mad.h"

#include <glm/gtc/type_ptr.hpp>

// Unused constructor
Shader::Shader()
{

}

// Default constructor we use
Shader::Shader(const std::string& vertexString, const std::string& fragmentString)
{
    // Changes string to char* since openGL needs it as char*
    const char* vShaderCode = vertexString.c_str();
    const char* fShaderCode = fragmentString.c_str();

    // Create and compile vertex shader
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    int success;
    char infoLog[512];

    // Catch error
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        LOG_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED...\n%s", infoLog);
    }

    LOG_DEBUG("Compiled vertex shader");

    // Create and compile fragment shader
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // Catch error
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        LOG_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED...\n%s", infoLog);
    }

    LOG_DEBUG("Compiled fragment shader");

    // Creates shader program, and attaches and links it
    m_program_number = glCreateProgram();
    glAttachShader(m_program_number, vertex);
    glAttachShader(m_program_number, fragment);
    glLinkProgram(m_program_number);

    // Catch error
    glGetProgramiv(m_program_number, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_program_number, 512, NULL, infoLog);
        LOG_ERROR("ERROR::SHADER::PROGRAM::LINK_FAILED...\n%s", infoLog);
    }

    LOG_DEBUG("Attached shaders to shaderprogram");

    // Deletes vertex and fragment since they are now linked to shader program
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const std::string& name)
{
    std::string vertexString;
    std::string fragmentString;

    std::string vertPath = "../resources/shaders/" + name + ".vert";
    std::string fragPath = "../resources/shaders/" + name + ".frag";

    mad::readStringFromFile(vertPath.c_str(), vertexString);
    mad::readStringFromFile(fragPath.c_str(), fragmentString);
    

    // Changes string to char* since openGL needs it as char*
    const char* vShaderCode = vertexString.c_str();
    const char* fShaderCode = fragmentString.c_str();

    // Create and compile vertex shader
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    int success;
    char infoLog[512];

    // Catch error
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        LOG_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED...\n%s", infoLog);
    }

    LOG_DEBUG("Compiled vertex shader");

    // Create and compile fragment shader
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // Catch error
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        LOG_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED...\n%s", infoLog);
    }

    LOG_DEBUG("Compiled fragment shader");

    // Creates shader program, and attaches and links it
    m_program_number = glCreateProgram();
    glAttachShader(m_program_number, vertex);
    glAttachShader(m_program_number, fragment);
    glLinkProgram(m_program_number);

    // Catch error
    glGetProgramiv(m_program_number, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_program_number, 512, NULL, infoLog);
        LOG_ERROR("ERROR::SHADER::PROGRAM::LINK_FAILED...\n%s", infoLog);
    }

    LOG_DEBUG("Attached shaders to shaderprogram");

    // Deletes vertex and fragment since they are now linked to shader program
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{   // Use this shaders
    glUseProgram(m_program_number);
}

// Sets a single int
void Shader::setInt(const std::string& name, GLint value)
{
    use();
    glUniform1i(glGetUniformLocation(m_program_number, name.c_str()), value);
}

// Overloaded function, takes 2d array of size 36 by 28 and put's it in a uniform 1d array of size 36*28
void Shader::setInt(const std::string& name, int value[36][28])
{
    use();
    {
        auto a = glGetUniformLocation(m_program_number, name.c_str());
        glUniform1iv(a, 28 * 36, &value[0][0]);
    }
}

// Sets a ivec2 / uvec2
void Shader::setInt(const std::string& name, GLint value, GLint value2)
{
    use();
    glUniform2i(glGetUniformLocation(m_program_number, name.c_str()), value, value2);
}

// Sets a ivec3 / uvec3
void Shader::setInt(const std::string& name, GLint value, GLint value2, GLint value3)
{
    use();
    glUniform3i(glGetUniformLocation(m_program_number, name.c_str()), value, value2, value3);
}

// Sets a ivec4 / uvec4
void Shader::setInt(const std::string& name, GLint value, GLint value2, GLint value3, GLint value4)
{
    use();
    glUniform4i(glGetUniformLocation(m_program_number, name.c_str()), value, value2, value3, value4);
}

// Sets a ivec2 / uvec2
void Shader::setInt(const std::string& name, glm::ivec2 vec)
{
    use();
    glUniform2i(glGetUniformLocation(m_program_number, name.c_str()), vec.x, vec.y);
}

// Sets a ivec3 / uvec3
void Shader::setInt(const std::string& name, glm::ivec3 vec)
{
    use();
    glUniform3i(glGetUniformLocation(m_program_number, name.c_str()), vec.x, vec.y, vec.z);
}

// Sets a ivec4 / uvec4
void Shader::setInt(const std::string& name, glm::ivec4 vec)
{
    use();
    glUniform4i(glGetUniformLocation(m_program_number, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

// Sets a float
void Shader::setFloat(const std::string& name, GLfloat value)
{
    use();
    glUniform1f(glGetUniformLocation(m_program_number, name.c_str()), value);
}

// Sets a vec2
void Shader::setFloat(const std::string& name, glm::vec2 vec)
{
    use();
    glUniform2f(glGetUniformLocation(m_program_number, name.c_str()), vec.x, vec.y);
}

// Sets a vec3
void Shader::setFloat(const std::string& name, glm::vec3 value)
{
    use();
    glUniform3f(glGetUniformLocation(m_program_number, name.c_str()), value.x, value.y, value.z);
}

// Sets a vec4
void Shader::setFloat(const std::string& name, glm::vec4 value)
{
    use();
    glUniform4f(glGetUniformLocation(m_program_number, name.c_str()), value.r, value.g, value.b, value.a);
}

// Sets a mat4
void Shader::setMatrix(const std::string& name, glm::mat4 trans)
{
    use();
    glUniformMatrix4fv(glGetUniformLocation(m_program_number, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
}

// Sets a mat3
void Shader::setMatrix(const std::string& name, glm::mat3 trans)
{
    use();
    glUniformMatrix3fv(glGetUniformLocation(m_program_number, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
}