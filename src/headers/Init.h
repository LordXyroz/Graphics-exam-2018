#pragma once
#include <iostream>
#include <vector>
#include "src/headers/Mesh.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool initWindow();

void inputHandler (GLFWwindow* window, int key, int scancode, int action, int mods);
void objloadertest(Mesh* mesh, std::string path);

void windowsize_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
