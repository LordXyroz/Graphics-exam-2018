#pragma once
#include "GameObject.h"
#include "Enums.h"
#include "glm/glm.hpp"

class Camera : public GameObject
{
private:
    CameraMode mode;

    float rotationAngle;
    float rotationSpeed;

    glm::vec3 followDistance;

    glm::vec3 upDirection;
    glm::vec3 xDirection;

    glm::vec3 rotationDirection;
    glm::vec3 lookatPosition;

    glm::mat4 projection;
    glm::mat4 view;

    GameObject* target;     // TODO, change to gameobject

public:
    Camera(glm::vec3 pos, glm::vec3 viewDir, glm::vec3 upDir, CameraMode newMode);

    void update();

    void input(int key, int scancode, int action, int mods);

    void rotate(glm::vec3 dir);
    void tilt(glm::vec3 dir);

    void setCamMode(CameraMode newMode);

    void setFollowDistance(glm::vec3 newDistance);
    void setUpDirection(glm::vec3 newDirection);
    void setLookatDirection(glm::vec3 newDirection);
    void setLookatPosition(glm::vec3 newPosition);
    void setRotationDirection(glm::vec3 newDirection);
    void setRotationAngle(float angle);
    void setRotationSpeed(float speed);

    void setTarget(GameObject* newTarget);
    void updateProjection(int width, int height);
    
    GameObject* getTarget();

    glm::vec3 getUpDirection();
    glm::vec3 getLookatDirection();
    glm::vec3 getFollowDistance();
    glm::vec3 getLookatPosition();

    glm::mat4 getProjection();
    glm::mat4 getView();

};