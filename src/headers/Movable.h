#pragma once
#include "GameObject.h"

//--------------------------------------------------------------------
//	TEST CODE, PLEASE DISREGARD
//--------------------------------------------------------------------

// Base class for movable object by input

class Movable : public GameObject
{
private:
    float rotationAngle;
   

public:
    float speed = 1.0f;
    glm::vec3 xDirection;
    Movable();

    virtual void update();
    virtual void input(int key, int scancode, int action, int mods);

    glm::vec3 getUpDirection();

    void randomPos();
    void reset();
};
