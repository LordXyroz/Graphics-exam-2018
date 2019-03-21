#include "src/headers/Camera.h"
#include "src/headers/Globals.h"
#include "src/headers/logger.h"
#include "src/headers/Globals.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

Camera::Camera(glm::vec3 pos, glm::vec3 viewDir, glm::vec3 upDir, CameraMode newMode)
{
    transform.position = pos;
    lookatDirection = viewDir;
    upDirection = upDir;
    xDirection = glm::vec3(1.f, 0.f, 0.f);
    mode = newMode;

    rotationAngle = 45.0f;
    rotationSpeed = 0.25f;
    rotationDirection = glm::vec3(0, 1, 0);
    lookatPosition = glm::vec3(0, 0, 0);
    followDistance = glm::vec3(0, 0, 1);

    transform.rotation = glm::mat4(1.0f);
    
    view = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(fov), float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1f, 10000.0f);
}

void Camera::update()
{
    if (lockMouse && mode == freeCam)
    {
        double xpos, ypos;

        glfwGetCursorPos(g_window, &xpos, &ypos);

        float xdiff = abs(xpos - (SCREEN_WIDTH / 2.f)) / 8.f;
        float ydiff = abs(ypos - (SCREEN_HEIGHT / 2.f)) / 8.f;

        if (xpos < SCREEN_WIDTH / 2.0f)
        {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle) * float(g_deltatime) * xdiff, glm::vec3(0, 1, 0));
            lookatDirection = rotation * glm::vec4(lookatDirection, 1.0f);
            upDirection = rotation * glm::vec4(upDirection, 1.0f);
            xDirection = glm::cross(lookatDirection, upDirection);
        }
        if (xpos > SCREEN_WIDTH / 2.0f)
        {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle) * float(g_deltatime) * xdiff, glm::vec3(0, -1, 0));
            lookatDirection = rotation * glm::vec4(lookatDirection, 1.0f);
            upDirection = rotation * glm::vec4(upDirection, 1.0f);
            xDirection = glm::cross(lookatDirection, upDirection);
        }

        if (ypos > SCREEN_HEIGHT / 2.0f)
        {
            transform.rotation = glm::rotate(glm::mat4(1.0), float(glm::radians(rotationAngle) * g_deltatime * ydiff), -xDirection);
            lookatDirection = transform.rotation * glm::vec4(lookatDirection, 1.0);
            upDirection = glm::cross(xDirection, lookatDirection);
        }
        if (ypos < SCREEN_HEIGHT / 2.0f)
        {
            transform.rotation = glm::rotate(glm::mat4(1.0), float(glm::radians(rotationAngle) * g_deltatime * ydiff), xDirection);
            lookatDirection = transform.rotation * glm::vec4(lookatDirection, 1.0);
            upDirection = glm::cross(xDirection, lookatDirection);
        }

        glfwSetCursorPos(g_window, SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0);
    }

    SDL_Color white = { 255, 255, 255, 255 };
    switch (mode)
    {
    case freeCam:
        view = glm::lookAt(transform.position, transform.position + lookatDirection, upDirection);
        box.draw("Cam: FREE", white, glm::vec3(-10.7, -5.7, 0), left);
        break;

    case staticCam:
        view = glm::lookAt(transform.position, lookatPosition, upDirection);
        box.draw("Cam: STATIC", white, glm::vec3(-10.7, -5.7, 0), left);
        break;

    case followCam:
        transform.position = target->transform.position + glm::vec3(target->transform.rotation * glm::vec4(0.f, 3.f, 5.f, 1.f));

        view = glm::lookAt(transform.position,
                           target->transform.position + target->lookatDirection + glm::vec3(target->transform.rotation * glm::vec4(0, 1, 0, 1)),
                           target->getUpDirection());
        box.draw("Cam: CHASE", white, glm::vec3(-10.7, -5.7, 0), left);
        break;

    case trackingCam:
        view = glm::lookAt(transform.position, target->transform.position, upDirection);
        box.draw("Cam: TRACKING", white, glm::vec3(-10.7, -5.7, 0), left);
        break;

    case orbitTargetCam:
        // Orbit gameobject
        transform.rotation = glm::rotate(glm::mat4(1.0f), float(glm::radians(rotationAngle) * g_deltatime * rotationSpeed), rotationDirection);
        
        followDistance = transform.rotation * glm::vec4(followDistance, 1.0);
        transform.position = target->transform.position + followDistance;
        view = glm::lookAt(transform.position, target->transform.position, upDirection);
        box.draw("Cam: ORBIT TARGET", white, glm::vec3(-10.7, -5.7, 0), left);
        break;

    case orbitPositionCam:
        transform.rotation = glm::rotate(glm::mat4(1.0f), float(glm::radians(rotationAngle) * g_deltatime * rotationSpeed), rotationDirection);
        
        followDistance = transform.rotation * glm::vec4(followDistance, 1.0);
        transform.position = lookatPosition + followDistance;
        view = glm::lookAt(transform.position, lookatPosition, upDirection);

        box.draw("Cam: ORBIT POINT", white, glm::vec3(-10.7, -5.7, 0), left);
        break;
    }
    projection = glm::perspective(glm::radians(fov), float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1f, 10000.0f);
}

void Camera::input(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        mode = static_cast<CameraMode>(int(mode) + 1);
        if (int(mode) > 5)
        {
            mode = freeCam;
            transform.position = glm::vec3(0, 0, 5);
        }

        lookatDirection = glm::vec3(0, 0, -1.0f);
        transform.rotation = glm::mat4(1.0f);
        upDirection = glm::vec3(0, 1, 0);
        xDirection = glm::cross(lookatDirection, upDirection);
    }

    if (mode == freeCam)
    {
        if (key == GLFW_KEY_I)
            transform.position += lookatDirection * 5.0f;
        if (key == GLFW_KEY_K)
            transform.position -= lookatDirection * 5.0f;
        if (key == GLFW_KEY_J)
            transform.position -= xDirection * 5.0f;
        if (key == GLFW_KEY_L)
            transform.position += xDirection * 5.0f;
        if (key == GLFW_KEY_Y)
            transform.position += glm::vec3(0, 1.f, 0) * 5.0f;
        if (key == GLFW_KEY_H)
            transform.position -= glm::vec3(0, 1.f, 0) * 5.0f;
    }

    if (key == GLFW_KEY_SLASH && action == GLFW_PRESS)
        mode = (mode == followCam) ? freeCam : followCam;

    if (key == GLFW_KEY_N)
        fov -= 1.f;
    if (key == GLFW_KEY_M)
        fov += 1.f;

    if (fov < 45.f) fov = 45.f;
    if (fov > 120.f) fov = 120.f;
}

void Camera::rotate(glm::vec3 dir)
{

}

void Camera::tilt(glm::vec3 dir)
{

}

void Camera::setCamMode(CameraMode newMode)
{
    mode = newMode;
}

void Camera::setFollowDistance(glm::vec3 newDistance)
{
    followDistance = newDistance;
}

void Camera::setUpDirection(glm::vec3 newDirection)
{
    upDirection = newDirection;
}

void Camera::setLookatDirection(glm::vec3 newDirection)
{
    lookatDirection = newDirection;
}

void Camera::setLookatPosition(glm::vec3 newPosition)
{
    lookatPosition = newPosition;
}

void Camera::setTarget(GameObject* newTarget)
{
    target = newTarget;
}

void Camera::setRotationDirection(glm::vec3 newDirection)
{
    rotationDirection = newDirection;
}

void Camera::setRotationAngle(float angle)
{
    rotationAngle = angle;
}

void Camera::setRotationSpeed(float speed)
{
    rotationSpeed = speed;
}

void Camera::updateProjection(int width, int height)
{
    projection = glm::perspective(glm::radians(fov), float(width) / float(height), 0.1f, 10000.f);
}

glm::vec3 Camera::getFollowDistance()
{
    return followDistance;
}

glm::vec3 Camera::getUpDirection()
{
    return upDirection;
}

glm::vec3 Camera::getLookatDirection()
{
    return lookatDirection;
}

glm::vec3 Camera::getLookatPosition()
{
    return lookatPosition;
}

glm::mat4 Camera::getProjection()
{
    return projection;
}

glm::mat4 Camera::getView()
{
    return view;
}

GameObject* Camera::getTarget()
{
    return target;
}