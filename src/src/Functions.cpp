#include "src/headers/Functions.h"
#include "src/headers/ObjectHandler.h"
#include "src/headers/Init.h"
#include "src/headers/Globals.h"
#include "src/headers/Camera.h"
#include "src/headers/logger.h"

#include "src/headers/Movable.h"
#include "src/headers/TextBox.h"

#define AUDIO_IMPLEMENTATION
#include "src/headers/Audio.hpp"

GameObject* map;
Movable* glider;

//--------------------------------------------------------------------
//	USED TO INITIALIZE THE GAME AND YOUR GAMEOBJECTS, CLASSES, AND FUNCTIONS
//--------------------------------------------------------------------
void Start()
{
    /////////////////////////////////////////////////////////////////////
    Audio::InitAudio();
    Audio::AddBGM("../resources/sound/victory.wav", "bgm", false);
    /////////////////////////////////////////////////////////////////////
    /// Meshes
    Mesh* mapMesh;

    if (!HIGH_RES_NO_NORMALS)
        mapMesh = LoadHeightMap("height20");// Able to load height100 and height50
    else
        mapMesh = new Mesh();

    Mesh* gliderMesh = LoadObject("ask21mi");
    /////////////////////////////////////////////////////////////////////
    /// Textures
	Texture* tex = LoadTexture("cherry");
	Texture* texTest = LoadTexture("texture");
    /////////////////////////////////////////////////////////////////////
    /// Shaders
	Shader* standardShader = LoadShader("standard");
	Shader* BaWShader = LoadShader("BaW");
    Shader* mapShader = LoadShader("map");
    /////////////////////////////////////////////////////////////////////
    /// TextBox
    box.initBox();
    /////////////////////////////////////////////////////////////////////
    /// Glider
	glider = new Movable();

	InitGameObject(glider, gliderMesh, texTest, standardShader);

    glider->transform.position = glm::vec3(0.0f, 10.0f, 0.0f);
    /////////////////////////////////////////////////////////////////////
    /// Terrain
    map = new GameObject();
    InitGameObject(map, mapMesh, tex, mapShader);

    /// Loads a high res map if toggled further up, sets a uniform
    /// to enable/disable diffuse lighting
    if (HIGH_RES_NO_NORMALS)
    {
        map->initHeightMap("height10"); // High res maps
        map->material.shader->setInt("noNormals", true);
    }
    else
        map->material.shader->setInt("noNormals", false);

    map->transform.scale = glm::vec3(4000.0f);
    map->transform.position = glm::vec3(-2000.0f, -500.0f, -4000.0f);
    map->material.specularity = 2048.f;
    /////////////////////////////////////////////////////////////////////
    /// Light
	Light* light = CreateLight(glm::vec3(1.0f));
    light->color = glm::vec3(0.222f, 0.432f, 0.534f);
    /////////////////////////////////////////////////////////////////////
    /// Camera
    mainCamera = new Camera(glm::vec3(0, 50, 0), glm::vec3(0, 0.0f, -1.0f), glm::vec3(0, 1, 0), freeCam);
    InitGameObject(mainCamera, nullptr, nullptr, nullptr);

    mainCamera->setTarget(glider);
	mainCamera->setFollowDistance(glm::vec3(0, 5, 5));
	mainCamera->setLookatPosition(glm::vec3(0, 0, 0));

    /////////////////////////////////////////////////////////////////////
    /// Initializing of variables
    float percent = (maxHeight - minHeight) / 100;
    /// Height of different zones
    normalWaterHeight = 15 * percent + minHeight;
    normalBeachHeight = 2 * percent + normalWaterHeight;
    normalVegitationHeight = 35 * percent + normalBeachHeight;
    normalMountainHeight = 25 * percent + normalVegitationHeight;

    springWater = (15 * percent + minHeight) * 1.25f;
    winterWaterHeight = (15 * percent + minHeight) * 0.85f;

    winterVegitation = (35 * percent + normalBeachHeight) * 0.75f;
    winterMountain = (25 * percent + normalVegitationHeight) * 0.75f;

    waterHeight = normalWaterHeight;
    beachHeight = normalBeachHeight;
    vegitationHeight = normalVegitationHeight;
    mountainHeight = normalMountainHeight;
    /// Colors of the different zones
    snowColor = glm::vec4(1.0, 1.0, 1.0, 1.0);

    waterColor = normalWater;
    beachColor = normalBeach;
    vegitationColor = normalVegitation;
    mountainColor = normalMountain;
    /////////////////////////////////////////////////////////////////////
    /// Centers cursor
    glfwSetCursorPos(g_window, SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0);
}

//--------------------------------------------------------------------
//	USED TO UPDATE THE GAME, OTHERWISE THE SAME AS START
//--------------------------------------------------------------------
void Update()
{
    /////////////////////////////////////////////////////////////////////
    seasonalSpeed = 16.f;   /// Seconds per season
    timeSpeed = 1.f;        /// Seconds per phase of day

    SDL_Color white = { 255, 255, 255, 255 };
    /////////////////////////////////////////////////////////////////////
    /// Respective update functions
    if (!pauseSeason)
        seasonalChange();
    if (!pauseTime)
        timeChange();
    /////////////////////////////////////////////////////////////////////
    switch (timeOfDay)
    {
    case 0:
        box.draw("MORNING", white, glm::vec3(10.7, 5.7, 0), right);
        break;
    case 1:
        box.draw("DAY", white, glm::vec3(10.7, 5.7, 0), right);
        break;
    case 2:
        box.draw("EVENING", white, glm::vec3(10.7, 5.7, 0), right);
        break;
    case 3:
        box.draw("NIGHT", white, glm::vec3(10.7, 5.7, 0), right);
        break;
    }
    /////////////////////////////////////////////////////////////////////
    switch (season)
    {
    case 0:
        box.draw("WINTER", white, glm::vec3(-10.7, 5.7, 0), left);
        break;
    case 1:
        box.draw("SPRING", white, glm::vec3(-10.7, 5.7, 0), left);
        break;
    case 2:
        box.draw("SUMMER", white, glm::vec3(-10.7, 5.7, 0), left);
        break;
    case 3:
        box.draw("AUTUMN", white, glm::vec3(-10.7, 5.7, 0), left);
        break;
    }
    /////////////////////////////////////////////////////////////////////
    /// Hidden and locked mouse can be toggled by pressing TAB
    if (lockMouse)
        glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    /////////////////////////////////////////////////////////////////////
}

void seasonalChange()
{
    /////////////////////////////////////////////////////////////////////
    /// Changes season
    seasonValue += g_deltatime;
    float sine = sin(seasonValue / seasonalSpeed);
    float cosine = cos(seasonValue / seasonalSpeed);
    /////////////////////////////////////////////////////////////////////
    if ((sine > -1 && sine < 0) && (cosine > 0 && cosine < 1))
        season = 0;     /// Winter
    if ((sine > 0 && sine < 1) && (cosine > 0 && cosine < 1))
        season = 1;     /// Spring
    if ((sine > 0 && sine < 1) && (cosine > -1 && cosine < 0))
        season = 2;     /// Summer
    if ((sine > -1 && sine < 0) && (cosine > -1 && cosine < 0))
        season = 3;     /// Autumn
    /////////////////////////////////////////////////////////////////////
    cosine = abs(cosine);
    sine = abs(sine);
    /////////////////////////////////////////////////////////////////////
    /// Interpolation based on season
    /// interpolate from - to
    /// Will interpolate nicely if season is changed manually
    switch (season)
    {
    case 0: /// Winter
        waterColor = interpolateColor(waterColor, winterWater, cosine * float(g_deltatime));
        beachColor = interpolateColor(beachColor, winterBeach, cosine * float(g_deltatime));
        vegitationColor = interpolateColor(vegitationColor, snowColor, cosine * float(g_deltatime));
        mountainColor = interpolateColor(mountainColor, snowColor, cosine * float(g_deltatime));

        waterHeight = interpolateHeight(waterHeight, winterWaterHeight, cosine * float(g_deltatime));
        vegitationHeight = interpolateHeight(vegitationHeight, winterVegitation, cosine * float(g_deltatime));
        mountainHeight = interpolateHeight(mountainHeight, winterMountain, cosine* float(g_deltatime));

        map->material.shader->setInt("enableSpec", 1);
        break;
    /////////////////////////////////////////////////////////////////////
    case 1: /// Spring
        waterColor = interpolateColor(waterColor, normalWater, sine * float(g_deltatime));
        beachColor = interpolateColor(beachColor, normalBeach, sine * float(g_deltatime));
        vegitationColor = interpolateColor(vegitationColor, normalVegitation, sine * float(g_deltatime));
        mountainColor = interpolateColor(mountainColor, normalMountain, sine * float(g_deltatime));

        waterHeight = interpolateHeight(waterHeight, springWater, sine * float(g_deltatime));
        vegitationHeight = interpolateHeight(vegitationHeight, normalVegitationHeight, sine * float(g_deltatime));
        mountainHeight = interpolateHeight(mountainHeight, normalMountainHeight, sine * float(g_deltatime));

        map->material.shader->setInt("enableSpec", 0);
        break;
    /////////////////////////////////////////////////////////////////////
    case 2: /// Summer
        waterColor = interpolateColor(waterColor, normalWater, cosine * float(g_deltatime));
        beachColor = interpolateColor(beachColor, normalBeach, cosine * float(g_deltatime));
        vegitationColor = interpolateColor(vegitationColor, normalVegitation, cosine * float(g_deltatime));
        mountainColor = interpolateColor(mountainColor, normalMountain, cosine * float(g_deltatime));

        waterHeight = interpolateHeight(waterHeight, normalWaterHeight, cosine * float(g_deltatime));
        vegitationHeight = interpolateHeight(vegitationHeight, normalVegitationHeight, cosine * float(g_deltatime));
        mountainHeight = interpolateHeight(mountainHeight, normalMountainHeight, cosine * float(g_deltatime));

        break;
    /////////////////////////////////////////////////////////////////////
    case 3: /// Autumn
        waterColor = interpolateColor(waterColor, normalWater, sine * float(g_deltatime));
        beachColor = interpolateColor(beachColor, normalBeach, sine * float(g_deltatime));
        mountainColor = interpolateColor(mountainColor, normalMountain, sine * float(g_deltatime));
        vegitationColor = interpolateColor(vegitationColor, autumnVegitation, sine * float(g_deltatime));

        waterHeight = interpolateHeight(waterHeight, normalWaterHeight, sine * float(g_deltatime));
        vegitationHeight = interpolateHeight(vegitationHeight, winterVegitation, sine * float(g_deltatime) * 0.25f);
        mountainHeight = interpolateHeight(mountainHeight, winterMountain, sine * float(g_deltatime) * 0.25f);

        break;
    }
    /////////////////////////////////////////////////////////////////////
    /// Uniforms
    map->material.shader->setFloat("waterHeight", waterHeight);
    map->material.shader->setFloat("beachHeight", beachHeight);
    map->material.shader->setFloat("vegitationHeight", vegitationHeight);
    map->material.shader->setFloat("mountainHeight", mountainHeight);
    map->material.shader->setFloat("waterColor", waterColor);
    map->material.shader->setFloat("beachColor", beachColor);
    map->material.shader->setFloat("vegitationColor", vegitationColor);
    map->material.shader->setFloat("mountainColor", mountainColor);
    map->material.shader->setFloat("snowColor", snowColor);
    map->material.shader->setInt("hardlines", hardlines);
    map->material.shader->setFloat("maxHeight", maxHeight);
    /////////////////////////////////////////////////////////////////////
}

void timeChange()
{
    /////////////////////////////////////////////////////////////////////
    /// Change time of day
    timeValue += g_deltatime;
    float sine = sin(timeValue / timeSpeed);
    float cosine = cos(timeValue / timeSpeed);
    SDL_Color white = { 255, 255, 255, 255 };
    /////////////////////////////////////////////////////////////////////
    lights[0]->transform.position = glm::vec3(1000 * sine, 1000 * cosine, 0);
    /////////////////////////////////////////////////////////////////////
    /// Small offset to get the transition right
    sine = sin(timeValue / timeSpeed + 0.5);
    cosine = cos(timeValue / timeSpeed + 0.5);
    /////////////////////////////////////////////////////////////////////
    if ((sine > -1 && sine < 0) && (cosine > 0 && cosine < 1))
        timeOfDay = 0;  /// Morning
    if ((sine > 0 && sine < 1) && (cosine > 0 && cosine < 1))
        timeOfDay = 1;  /// Noon
    if ((sine > 0 && sine < 1) && (cosine > -1 && cosine < 0))
        timeOfDay = 2;  /// Evening
    if ((sine > -1 && sine < 0) && (cosine > -1 && cosine < 0))
        timeOfDay = 3;  /// Midnight
    /////////////////////////////////////////////////////////////////////
}

glm::vec4 interpolateColor(glm::vec4 color1, glm::vec4 color2, float factor)
{
    return (1.0f - factor) * color1 + factor * color2;
}

float interpolateHeight(float val1, float val2, float factor)
{
    return (1.0f - factor) * val1 + factor * val2;
}