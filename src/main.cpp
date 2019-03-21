#include <iostream>
#include "headers/Init.h"
#include "headers/Camera.h"
#include "headers/Functions.h"
#include "headers/Globals.h"
#include "headers/ObjectHandler.h"
#include "headers/logger.h"
#include "headers/Audio.hpp"
#include "headers/Movable.h"

#include <SDL.h>
#undef main

extern Movable* glider;

int main()
{
    /////////////////////////////////////////////////////////////////////
    /// Change this to true to use a lower poly map
    HIGH_RES_NO_NORMALS = true;
    ////////////////////////////////////////////////////////////////////
	//	Initializing
	SDL_Init(SDL_INIT_AUDIO);
    initWindow();
	Start();

	//	deltaTime
    double currentframe = glfwGetTime();
    double lastframe = currentframe;
    double lastframe2 = currentframe;

    while (!glfwWindowShouldClose(g_window))
    {
		//	deltaTime
        currentframe = glfwGetTime();
        g_deltatime = currentframe - lastframe;
        double dt2 = currentframe - lastframe2;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	Update functions

		Update();
		UpdateGameObjects();
		
        if (dt2 > 1.0)
        {
            LOG_DEBUG("FPS: %f", 1 / g_deltatime);
            lastframe2 = currentframe;
        }

		//	Draw all GameObjects
		DrawGameObjects();
        glfwSwapBuffers(g_window);

        glfwPollEvents();

        lastframe = currentframe;
    }

    glfwTerminate();

	return 0;
}
