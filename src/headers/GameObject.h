#pragma once
#include <string>
#include "Structs.h"
#include "src/headers/objectLoader.hpp"

// Base class for GameObjects in the project

class GameObject
{
public:
    Transform transform;        // Position, Rotation, Scale
	Mesh* mesh;
	Mesh* prevMesh;
	Buffer* buffer;
	Material material;

    int num;

    glm::vec3 upDirection;
    glm::vec3 lookatDirection;

	GameObject(glm::vec3 pos = glm::vec3(0.0f), glm::mat4 rot = glm::mat4(1.0f), glm::vec3 scale = glm::vec3(1.0f));
	~GameObject();

	virtual void init(Mesh* obj, Texture* tex, Shader* shade) final;
    virtual void draw() final;
	virtual void setMeshBuffers() final;

    virtual void update();
    virtual void input(int key, int scancode, int action, int mods);

    glm::vec3 getUpDirection();

    void initHeightMap(std::string fileName);
};
