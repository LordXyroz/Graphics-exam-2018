#include "src/headers/Globals.h"

bool HIGH_RES_NO_NORMALS = false;

int SCREEN_WIDTH = 1600;
int SCREEN_HEIGHT = 900;

double g_deltatime;
Camera* mainCamera;
GLFWwindow* g_window;
TextBox box;

float fov = 45.f;

float maxHeight = 0.0f;
float minHeight = 1.0f;

float normalWaterHeight;
float normalBeachHeight;
float normalVegitationHeight;
float normalMountainHeight;

float springWater;
float winterWaterHeight;

float winterVegitation;
float winterMountain;

float waterHeight;
float beachHeight;
float vegitationHeight;
float mountainHeight;

bool lockMouse = true;

float seasonalSpeed = 4.0f;
bool pauseSeason = false;
bool pauseTime = false;

float timeSpeed = 1.f;

float timeValue = 0.f;
float seasonValue = 0.f;

glm::vec4 snowColor = glm::vec4(1.0, 1.0, 1.0, 1.0);

glm::vec4 waterColor;
glm::vec4 beachColor;
glm::vec4 vegitationColor;
glm::vec4 mountainColor;

glm::vec4 winterWater = glm::vec4(54.0 / 255.0, 98.0 / 255.0, 186.0 / 255.0, 1.0);
glm::vec4 winterBeach = glm::vec4(1.0, 1.0, 0.5, 0);

glm::vec4 normalWater = glm::vec4(0.0, 0.0, 236.0 / 255.0, 1.0);
glm::vec4 normalBeach = glm::vec4(1.0, 0.7, 0.0, 1.0);
glm::vec4 normalVegitation = glm::vec4(0, 0.5, 0.0, 1.0);
glm::vec4 normalMountain = glm::vec4(183.0 / 255.0, 117.0 / 255.0, 81.0 / 255.0, 1.0);

glm::vec4 autumnVegitation = glm::vec4(0.5, 0.25, 0.0, 1.0);

int season = 0;
int timeOfDay = 0;
int hardlines = 0;
int fog = 0;