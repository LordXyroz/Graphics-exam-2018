#include "src/headers/TextBox.h"
#include "src/headers/ObjectHandler.h"
#include "src/headers/Globals.h"

// Default constructor we use
TextBox::TextBox()
{

}

void TextBox::initBox()
{
    mesh = new Mesh();
    buffer = new Buffer();
    material.shader = new Shader("text");
    material.texture = new Texture();

    font = TTF_OpenFont("../resources/fonts/Joystix.TTF", 60);     // Loads the font
    projection = glm::perspective(glm::radians(45.f), float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1f, 10000.0f);

    glGenVertexArrays(1, &buffer->VAO);

    glGenBuffers(1, &buffer->VBO[0]);                   // Generates unique ID for buffer object
    glGenBuffers(1, &buffer->IBO);                      // Generates unique ID for Element object
}

void TextBox::draw(std::string text, SDL_Color color, glm::vec3 pos, Allignment allign)
{
    // Width used for the width of the drawn rectangle
    int width = material.texture->createTextureFromText(font, text, color); // Creates texture from input string

    // Mesh creation
    std::vector<glm::vec3> vertices;

    // Vertices based on allignment
    if (allign == center)
    {
        vertices = {
            glm::vec3((width / 20) / 2,  1.0f,  0.0f),  // Top Right
            glm::vec3((width / 20) / 2,  0.0f,  0.0f),  // Bottom Right
            glm::vec3((-width / 20) / 2, 0.0f,  0.0f),  // Bottom Left
            glm::vec3((-width / 20) / 2, 1.0f,  0.0f),  // Top Left
        };
    }
    if (allign == left)
    {
        vertices = {
            glm::vec3((width / 20),  1.0f,  0.0f),      // Top Right
            glm::vec3((width / 20),  0.0f,  0.0f),      // Bottom Right
            glm::vec3(0.0f, 0.0f,  0.0f),               // Bottom Left
            glm::vec3(0.0f, 1.0f,  0.0f),               // Top Left
        };
    }

    if (allign == right)
    {
        vertices = {
            glm::vec3(0.0f,  1.0f,  0.0f),              // Top Right
            glm::vec3(0.0f,  0.0f,  0.0f),              // Bottom Right
            glm::vec3((-width / 20), 0.0f,  0.0f),      // Bottom Left
            glm::vec3((-width / 20), 1.0f,  0.0f),      // Top Left
        };
    }

    // Texture coordinates
    std::vector<glm::vec2> texCoords = {

        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
    };

    // Indices
    std::vector<glm::uvec3> indices = {
        glm::uvec3(3, 1, 0),
        glm::uvec3(3, 2, 1),
    };

    mesh->vertices = vertices;
    mesh->texCoord = texCoords;
    mesh->verticesIndices = indices;

    glBindVertexArray(buffer->VAO);                                             // Binds VAO for use

    glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO[0]);                              // Binds unique buffer to a target, GL_ARRAY_BUFFER here
    // Puts all the data in a single buffer since it's only 1 square
    glBufferData(GL_ARRAY_BUFFER, (mesh->vertices.size() * sizeof(glm::vec3) + mesh->texCoord.size() * sizeof(glm::vec3)), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->vertices.size() * sizeof(glm::vec3), mesh->vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(glm::vec3), mesh->texCoord.size() * sizeof(glm::vec3), mesh->texCoord.data());


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);   // How to interpret the vertex data
    // Which vertex attribute, number of dimensions in the vector, type of data, normalized or not, space between attribute sets, offset

    glEnableVertexAttribArray(0); // <-------------------------------- Remember 

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(mesh->vertices.size() * sizeof(glm::vec3))); // How to interpret the vertex data
    // Which vertex attribute, number of dimensions in the vector, type of data, normalized or not, space between attribute sets, offset

    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->IBO);                     // Binds unique buffer to a target, GL_ELEMENT_ARRAY_BUFFER here
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->verticesIndices.size() * sizeof(glm::uvec3), mesh->verticesIndices.data(), GL_STATIC_DRAW);
    // Copies indices data into the GL_ELEMENT_ARRAY_BUFFER target

 
    glm::mat4 model = glm::mat4(1.0f);              // Identity matrix
    // uses a different view matrix than normal entities
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f));

    model = glm::translate(model, pos);             // Translates local coordinates to world space
    model = glm::scale(model, glm::vec3(0.13f, 0.3f, 0.3f));

    material.shader->use();                        // Uses bound shader
    material.shader->setMatrix("model", model);    // Sets model uniform in shader
    material.shader->setMatrix("view", view);      // Sets view uniform in shader
    material.shader->setMatrix("projection", projection);

    glBindTexture(GL_TEXTURE_2D, material.texture->getNo());   // Uses the texture we created from the string
    glBindVertexArray(buffer->VAO);                             // Binds VAO for draw
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->IBO);         // Binds IBO for draw

    glDrawElements(GL_TRIANGLES, mesh->verticesIndices.size() * sizeof(glm::uvec3), GL_UNSIGNED_INT, 0); // Draws elements
}