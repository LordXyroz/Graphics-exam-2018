#pragma once
#include "src/headers/Movable.h"
#include "src/headers/Globals.h"
#include "src/headers/ObjectHandler.h"

//--------------------------------------------------------------------
//	TEST CODE, PLEASE DISREGARD
//--------------------------------------------------------------------

Movable::Movable()
{
    upDirection = glm::vec3(0, 1.f, 0);
    lookatDirection = glm::vec3(0, 0, -1.f);
    xDirection = glm::vec3(1.f, 0.f, 0.f);

    rotationAngle = 45.0f;

    transform.rotation = glm::mat4(1.0f);
}

void Movable::update()
{
    SDL_Color white = { 255, 255, 255, 255 };
    box.draw("SPEED: " + std::to_string(speed), white, glm::vec3(10.7, -5.7, 0), right);

    transform.position += lookatDirection * speed / 10.f;
}

void Movable::input(int key, int scancode, int action, int mods)
{
    /*   
    if (key == GLFW_KEY_W)
    {
        if (xDirection.y > -0.1 && xDirection.y < 0.1)
        {
            transform.rotation = glm::rotate(glm::mat4(1.0), float(glm::radians(rotationAngle) * g_deltatime), -xDirection);
            lookatDirection = transform.rotation * glm::vec4(lookatDirection, 1.0);
            upDirection = glm::cross(xDirection, lookatDirection);
        }
        else
        {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle) * float(g_deltatime), glm::vec3(0, -1, 0));
            transform.rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle) * float(g_deltatime), glm::vec3(0, -1, 0));
            lookatDirection = transform.rotation * glm::vec4(lookatDirection, 1.0f);
            upDirection = transform.rotation * glm::vec4(upDirection, 1.0f);
            xDirection = glm::cross(lookatDirection, upDirection);
        }
    }
    if (key == GLFW_KEY_S)
    {
        if (xDirection.y > -0.1 && xDirection.y < 0.1)
        {
            transform.rotation = glm::rotate(glm::mat4(1.0), float(glm::radians(rotationAngle) * g_deltatime), -xDirection);
            lookatDirection = transform.rotation * glm::vec4(lookatDirection, 1.0);
            upDirection = glm::cross(xDirection, lookatDirection);
        }
        else
        {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle) * float(g_deltatime), glm::vec3(0, 1, 0));
            transform.rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle) * float(g_deltatime), glm::vec3(0, 1, 0));
            lookatDirection = transform.rotation * glm::vec4(lookatDirection, 1.0f);
            upDirection = transform.rotation * glm::vec4(upDirection, 1.0f);
            xDirection = glm::cross(lookatDirection, upDirection);
        }
    }
    if (key == GLFW_KEY_A)
    {
        //glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position);
        glm::mat4 model = glm::mat4(1.0f) * transform.rotation;
        model = glm::inverse(model);

        glm::vec3 temp = model * glm::vec4(0, 0, 1, 1.0);

        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), float(glm::radians(rotationAngle) * g_deltatime), temp);
        transform.rotation = glm::rotate(transform.rotation, float(glm::radians(rotationAngle) * g_deltatime), temp);
        upDirection = rotation * glm::vec4(0.f, 1.f, 0.f, 1.0f);
        xDirection = glm::cross(lookatDirection, upDirection);
    }
    if (key == GLFW_KEY_D)
    {
        //glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position);
        glm::mat4 model = glm::mat4(1.0f) * transform.rotation;
        model = glm::inverse(model);

        glm::vec3 temp = model * glm::vec4(0, 0, -1, 1.0);
        
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), float(glm::radians(rotationAngle) * g_deltatime), temp);
        transform.rotation = glm::rotate(transform.rotation, float(glm::radians(rotationAngle) * g_deltatime), temp);
        upDirection = rotation * glm::vec4(0.f, 1.f, 0.f, 1.0f);
        xDirection = glm::cross(lookatDirection, upDirection);
    }
    */
    glm::vec3 temp = glm::vec3(0.0);

    if (key == GLFW_KEY_D)
        temp = glm::vec3(0, 0, -1);
    if (key == GLFW_KEY_A)
        temp = glm::vec3(0, 0, 1);

    if (key == GLFW_KEY_S)
        temp = glm::vec3(1, 0, 0);
    if (key == GLFW_KEY_W)
        temp = glm::vec3(-1, 0, 0);

    if (key == GLFW_KEY_Q)
        temp = glm::vec3(0, 1, 0);
    if (key == GLFW_KEY_E)
        temp = glm::vec3(0, -1, 0);
       

    if (temp.x != 0 || temp.y != 0 || temp.z != 0)
    {
        transform.rotate(45.f, temp, g_deltatime);
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(45.f) * float(g_deltatime), temp);
        lookatDirection = rot * glm::vec4(lookatDirection, 1.0);
        upDirection = rot * glm::vec4(upDirection, 1.0);
    }

    if (key == GLFW_KEY_PERIOD)
        speed += 0.05f;
    if (key == GLFW_KEY_COMMA)
        speed -= 0.05f;
    
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        randomPos();
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        reset();
}

void Movable::randomPos()
{
    transform.position.x = (rand() % 800 - 400);
    transform.position.z = (rand() % 1600 - 800);
}

void Movable::reset()
{
    transform.position = glm::vec3(0.0f, -10.0f, 0.0f);

    transform.rotation = glm::mat4(1.0f);

    upDirection = glm::vec3(0, 1.f, 0);
    lookatDirection = glm::vec3(0, 0, -1.f);
    xDirection = glm::cross(lookatDirection, upDirection);
}

