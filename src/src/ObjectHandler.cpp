#include "src/headers/ObjectHandler.h"
#include "src/headers/Globals.h"
#include "src/headers/logger.h"
#include "src/headers/mad.h"

#include "src/headers/stb_image.h"

#include <SDL2/include/SDL_surface.h>


std::vector<Light*> lights;
std::vector<GameObject*> gameObjects;
std::vector<Mesh*> meshes;
std::vector<Texture*> textures;
std::vector<Shader*> shaders;

Light* CreateLight(glm::vec3 pos, glm::vec3 color)
{
	Light* light = new Light();
	light->transform.position = pos;
	light->color = color;
	lights.push_back(light);

	return light;
}

void InitGameObject(GameObject* go, Mesh* obj, Texture* tex, Shader* shade)
{
	go->init(obj, tex, shade);
}

Mesh* LoadObject(std::string fileName)
{
	std::string path = "../resources/models/";

	Mesh* object = new Mesh(loadObject(path + fileName + ".obj")[0]);
	meshes.push_back(object);

	return object;
}

Mesh* LoadHeightMap(std::string fileName)
{
    std::string path = "../resources/heightmap/";

    Mesh* object = new Mesh();

    stbi_set_flip_vertically_on_load(true);                 // Makes sure the image is not upside down

    int width;
    int height;
    int nrChannels;

    unsigned char* data = stbi_load((path + fileName + ".png").c_str(), &width, &height, &nrChannels, 0);

    object->vertices.reserve(width*height);
    object->texCoord.reserve(width*height);
    object->verticesIndices.reserve(width*height*2);
    object->normals.reserve(width*height);

    if (data)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                object->vertices.emplace_back(glm::vec3(1 - (x / float(width)), data[width*(y * nrChannels) + (x * nrChannels)] / 2048.f, y / float(width)));
                object->texCoord.emplace_back(glm::vec2(1 - (x / float(width)), y / float(height)));
            }
        }

        for (auto vert : object->vertices)
        {
            if (vert.y > maxHeight) maxHeight = vert.y;
            if (vert.y < minHeight) minHeight = vert.y;
        }
        
        for (int y = 0; y < height - 1; y++)
        {
            for (int x = 0; x < width - 1; x++)
            {
                object->verticesIndices.emplace_back(glm::uvec3(1 + x + width * (y + 1), x + width * (y + 1), x + width * y));
                object->verticesIndices.emplace_back(glm::uvec3(1 + x + width * (y + 1), x + width * y, 1 + x + width * y));
            }
        }

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                glm::vec3 L = (x == 0) ? object->vertices[1 + x + width * y] : object->vertices[- 1 + x + width * y];
                glm::vec3 R = (x == width - 1) ? object->vertices[-1 + x + width * y] : object->vertices[1 + x + width * y];
                glm::vec3 B = (y == 0) ? object->vertices[x + width * (y + 1)] : object->vertices[x + width * (y - 1)];
                glm::vec3 T = (y == height - 1) ? object->vertices[x + width * (y - 1)] : object->vertices[x + width * (y + 1)];
                glm::vec3 RL = R - L;
                glm::vec3 BT = B - T;
                object->normals.emplace_back(glm::normalize(glm::cross(BT, RL)));
            }
        }
    }
    else
    {
        LOG_ERROR("ERROR::TEXTURE::LOADING..... From path: %s", path);
    }

    stbi_image_free(data);

    meshes.push_back(object);

    return object;
}

Texture* LoadTexture(std::string fileName)
{
	std::string path = "../resources/textures/";

	Texture* texture = new Texture((path + fileName + ".png").c_str());
	textures.push_back(texture);

	return texture;
}

Shader* LoadShader(std::string fileName)
{
	Shader* shader = new Shader(fileName);
	shaders.push_back(shader);

	return shader;
}

void UpdateGameObjects()
{
	for (int i=0; i<gameObjects.size(); i++)
		gameObjects[i]->update();
}

void DrawGameObjects()
{
	for (int i = 0; i<gameObjects.size(); i++)
		gameObjects[i]->draw();
}

void HandleInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	for (int i = 0; i<gameObjects.size(); i++)
		gameObjects[i]->input(key, scancode, action, mods);
}
