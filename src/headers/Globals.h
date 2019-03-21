#pragma once
#include "src/headers/Camera.h"
#include "src/headers/TextBox.h"

extern bool HIGH_RES_NO_NORMALS;

extern double g_deltatime;
extern Camera* mainCamera;
extern TextBox box;

extern float fov;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern GLFWwindow* g_window;

extern float minHeight;
extern float maxHeight;

extern float normalWaterHeight;
extern float normalBeachHeight;
extern float normalVegitationHeight;
extern float normalMountainHeight;

extern float springWater;
extern float winterWaterHeight;

extern float winterVegitation;
extern float winterMountain;

extern float waterHeight;
extern float beachHeight;
extern float vegitationHeight;
extern float mountainHeight;

extern float seasonalSpeed;
extern bool pauseSeason;
extern bool pauseTime;

extern float timeSpeed;

extern bool lockMouse;

extern float timeValue;
extern float seasonValue;

extern glm::vec4 snowColor;

extern glm::vec4 waterColor;
extern glm::vec4 beachColor;
extern glm::vec4 vegitationColor;
extern glm::vec4 mountainColor;

extern glm::vec4 winterWater;
extern glm::vec4 winterBeach;

extern glm::vec4 normalWater;
extern glm::vec4 normalBeach;
extern glm::vec4 normalVegitation;
extern glm::vec4 normalMountain;

extern glm::vec4 autumnVegitation;

extern int season;
extern int timeOfDay;

extern int hardlines;
extern int fog;