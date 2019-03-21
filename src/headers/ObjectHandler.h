#pragma once
#include <vector>
#include "src/headers/GameObject.h"
#include "src/headers/objectLoader.hpp"

extern std::vector<Light*> lights;
extern std::vector<GameObject*> gameObjects;
extern std::vector<Mesh*> meshes;
extern std::vector<Texture*> textures;
extern std::vector<Shader*> shaders;

Light* CreateLight(glm::vec3 pos, glm::vec3 color = glm::vec3(1.0f));
void InitGameObject(GameObject* go, Mesh* obj, Texture* tex, Shader* shade);
Mesh* LoadObject(std::string fileName);
Mesh* LoadHeightMap(std::string fileName);
Texture* LoadTexture(std::string fileName);
Shader* LoadShader(std::string fileName);

void UpdateGameObjects();
void DrawGameObjects();
void HandleInput(GLFWwindow* window, int key, int scancode, int action, int mods);
