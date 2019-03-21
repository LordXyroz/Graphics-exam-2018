#include "src/headers/GameObject.h"
#include "src/headers/mad.h"
#include "src/headers/Init.h"
#include "src/headers/Camera.h"
#include "src/headers/ObjectHandler.h"
#include "src/headers/logger.h"
#include "src/headers/Globals.h"
#include "glm/glm.hpp"
#include "src/headers/stb_image.h"

extern Camera* mainCamera;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

GameObject::GameObject(glm::vec3 pos, glm::mat4 rot, glm::vec3 scale)
{
	transform.position = pos; // Position 0
	transform.rotation = rot; // Identity matrix
	transform.scale = scale; // Scale 1
}

void GameObject::init(Mesh* obj, Texture* tex, Shader* shade)
{
	gameObjects.push_back(this);

	mesh = new Mesh;
	buffer = new Buffer;

	material.shader = shade;

	/*mesh->normals.resize(obj.normals.size());
	mesh->texture_coordinates.resize(obj.texCoord.size());*/
	mesh = obj;
	prevMesh = mesh;

	//objloadertest(mesh, obj.c_str());

	material.texture = tex;

    if (mesh != nullptr) setMeshBuffers();
}

void GameObject::draw()
{
    if (mesh != nullptr && material.shader != nullptr && material.texture != nullptr)
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, transform.position);
        model *= transform.rotation;
        model = glm::scale(model, transform.scale);

        material.shader->setMatrix("model", model);
        material.shader->setMatrix("view", mainCamera->getView());				//	GET FROM GLOBAL CAMERA
        material.shader->setMatrix("projection", mainCamera->getProjection());	//	GET FROM GLOBAL CAMERA
        material.shader->setFloat("lightPos", lights[0]->transform.position);	//	GET FROM GLOBAL LIGHT
        material.shader->setFloat("lightColor", lights[0]->color);				//	GET FROM GLOBAL LIGHT
        material.shader->setFloat("camPos", mainCamera->transform.position);		//	GET FROM GLOBAL CAMERA
        material.shader->setFloat("vertexColor", material.color);
        material.shader->setFloat("specularity", material.specularity);

        if (mesh != prevMesh)
        {
            //std::cout << "Mesh updated\n";
            setMeshBuffers();
            prevMesh = mesh;
        }
        else
            glBindVertexArray(buffer->VAO);                             // Bind VAO for drawing
        glBindTexture(GL_TEXTURE_2D, material.texture->getNo());

        
        if (mesh->verticesIndices.size() > 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->IBO);         // Bind IBO for drawing
            glDrawElements(GL_TRIANGLES, mesh->verticesIndices.size() * sizeof(glm::uvec3), GL_UNSIGNED_INT, 0);
        }
        else if (mesh->vertices.size() > 0)
            glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size() * sizeof(glm::vec3));
        else
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->IBO);
            glDrawElements(GL_TRIANGLES, num * sizeof(glm::uvec3), GL_UNSIGNED_INT, 0);
        }
        

    }
}

void GameObject::update()
{
    // To be implemented by child classes
}

GameObject::~GameObject()
{
	//	Do not delete mesh, mesh points to globally saved meshes
	//		Same goes for material.shader, and texture

	delete buffer;
}

void GameObject::input(int key, int scancode, int action, int mods)
{
	//	To be implemented by child classes
}

void GameObject::setMeshBuffers()
{
    num = mesh->vertices.size();
    if (mesh->vertices.size() > 0)
    {
        glGenVertexArrays(1, &buffer->VAO);                                         // Generate VAO with unique ID
        glBindVertexArray(buffer->VAO);                                             // Binds the VAO so changed to buffers will be stored

        glGenBuffers(3, &buffer->VBO[0]);                                           // Generates 3 unique ID for buffer object
        glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO[0]);                              // Binds unique buffer to a target, GL_ARRAY_BUFFER here

        glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO[0]);                              // Binds unique buffer to a target, GL_ARRAY_BUFFER here
        glBufferData(GL_ARRAY_BUFFER, (mesh->vertices.size() * sizeof(glm::vec3)), mesh->vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);               // How to interpret the vertex data
                                                                                    // Which vertex attribute, number of dimensions in the vector, type of data, normalized or not, space between attribute sets, offset

        glEnableVertexAttribArray(0); // <-------------------------------- Remember

        glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO[1]);                              // Binds unique buffer to a target, GL_ARRAY_BUFFER here
        glBufferData(GL_ARRAY_BUFFER, (mesh->texCoord.size() * sizeof(glm::vec2)), mesh->texCoord.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);               // How to interpret the vertex data
                                                                                    // Which vertex attribute, number of dimensions in the vector, type of data, normalized or not, space between attribute sets, offset

        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO[2]);                              // Binds unique buffer to a target, GL_ARRAY_BUFFER here
        glBufferData(GL_ARRAY_BUFFER, (mesh->normals.size() * sizeof(glm::vec3)), mesh->normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);               // How to interpret the vertex data
                                                                                    // Which vertex attribute, number of dimensions in the vector, type of data, normalized or not, space between attribute sets, offset

        glEnableVertexAttribArray(2);


        if (mesh->verticesIndices.size() > 0)
        {
            glGenBuffers(1, &buffer->IBO);                                          // Generates unique ID for Element object
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->IBO);                     // Binds unique buffer to a target, GL_ELEMENT_ARRAY_BUFFER here
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->verticesIndices.size() * sizeof(glm::uvec3), mesh->verticesIndices.data(), GL_STATIC_DRAW);
            // Copies indices data into the GL_ELEMENT_ARRAY_BUFFER target
        }

        // Unbinds everything
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void GameObject::initHeightMap(std::string fileName)
{
    std::string path = "../resources/heightmap/";

    stbi_set_flip_vertically_on_load(true);                 // Makes sure the image is not upside down

    int width;
    int height;
    int nrChannels;

    unsigned char* data = stbi_load((path + fileName + ".png").c_str(), &width, &height, &nrChannels, 0);

    int total = width * height;
    num = total;

    glGenVertexArrays(1, &buffer->VAO);                                         // Generate VAO with unique ID
    glBindVertexArray(buffer->VAO);                                             // Binds the VAO so changed to buffers will be stored

    glGenBuffers(3, &buffer->VBO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * total, nullptr, GL_STATIC_DRAW);

    //std::vector<glm::vec3> vertices;
    for (int y = 0; y < height; y++)
    {   
        std::vector<glm::vec3> vertices;

        for (int x = 0; x < width; x++)
        {
            vertices.push_back(glm::vec3(1 - (x / float(width)), data[width*(y * nrChannels) + (x * nrChannels)] / 1024.f, y / float(width)));
            if (data[width*(y * nrChannels) + (x * nrChannels)] / 1024.f > maxHeight) maxHeight = data[width*(y * nrChannels) + (x * nrChannels)] / 1024.f;
            if (data[width*(y * nrChannels) + (x * nrChannels)] / 1024.f < minHeight) minHeight = data[width*(y * nrChannels) + (x * nrChannels)] / 1024.f;
        }
        glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3) * y, vertices.size() * sizeof(glm::vec3), vertices.data());
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &buffer->IBO);                                          // Generates unique ID for Element object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->IBO);                     // Binds unique buffer to a target, GL_ELEMENT_ARRAY_BUFFER here
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, total * 2 * sizeof(glm::uvec3), nullptr, GL_STATIC_DRAW);

    for (int y = 0; y < height - 1; y++)
    {
        std::vector<glm::uvec3> indices;

        for (int x = 0; x < width - 1; x++)
        {
            indices.push_back(glm::uvec3(1 + x + width * (y + 1), x + width * (y + 1), x + width * y));
            indices.push_back(glm::uvec3(1 + x + width * (y + 1), x + width * y, 1 + x + width * y));
        }
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::vec3) * y, indices.size() * sizeof(glm::vec3), indices.data());
    }
    //glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), vertices.data());


    // LOG_DEBUG("size of vertices: %d", vertices.size());
}


glm::vec3 GameObject::getUpDirection()
{
    return upDirection;
}