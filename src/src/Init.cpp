#include "src/headers/Init.h"
#include "src/headers/logger.h"
#include "src/headers/mad.h"
#include "src/headers/Camera.h"
#include "src/headers/ObjectHandler.h"
#include "src/headers/Globals.h"
#include <vector>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>


#define TINYOBJLOADER_IMPLEMENTATION
#include "src/headers/tiny_obj_loader.h"

extern GLFWwindow* g_window;

extern Camera* mainCamera;

extern GameObject* map;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern glm::mat4 g_projection;
extern glm::mat4 g_view;

bool initWindow()
{
    glfwInit();                                                             // Inits glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                          // Sets major openGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                          // Sets minor openGL version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);          // Sets that we want to use core functionality
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    g_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Oblig 2", NULL, NULL);          // Creates a window, size 800x600, Learning 2 as title

    if (g_window == NULL)                                                     // If no window created, terminates program
    {
        LOG_ERROR("Error creating window, terminating...\n");
        glfwTerminate();
        return false;
    }

    LOG_DEBUG("Window created");

    glfwMakeContextCurrent(g_window);                                       // Sets window to be used
    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_FALSE);
    glfwSetKeyCallback(g_window, inputHandler);
    glewExperimental = GL_TRUE;                                             // For Linux/Mac OS

    if (glewInit() != GLEW_OK)                                              // Inits glew, terminates if not
    {
        LOG_ERROR("Failed to init glew, terminating...\n");
        glfwTerminate();
        return false;
    }

    LOG_DEBUG("Glew initiated");

    glEnable(GL_BLEND);                                                     // Enables use of blending
    glEnable(GL_DEPTH_TEST);                                                // Depth test so that only textures on top get rendered
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);                      // Enables the use of alpha channel
    
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);                          // Sets the viewport coordinates and size
    glfwSetFramebufferSizeCallback(g_window, windowsize_callback);          // Sets what function to call when screen gets resized
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);                                   // Sets a clear color / black here
    glClear(GL_COLOR_BUFFER_BIT);                                           // Clears buffer
    glfwSwapBuffers(g_window);                                              // Swaps buffer
    glClear(GL_COLOR_BUFFER_BIT);                                           // Clears buffer

	TTF_Init();
    glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    LOG_DEBUG("Initialization successful");
    return true;
}

// Callback function for when the window resizes
void windowsize_callback(GLFWwindow* window, int width, int height)
{

    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;

    mainCamera->updateProjection(width, height);
    glfwGetWindowSize(window, &height, &width);
    glViewport(0, 0, height, width);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

}


void inputHandler (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    HandleInput(g_window, key, scancode, action, mods);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(g_window, true);

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        hardlines++;
        if (hardlines > 1) hardlines = 0;
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (pauseSeason) pauseSeason = false;
        else pauseSeason = true;
    }
    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        if (pauseTime) pauseTime = false;
        else pauseTime = true;
    }
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        if (lockMouse) lockMouse = false;
        else
        {
            lockMouse = true;
            glfwSetCursorPos(g_window, SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0);
        }
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        fog++;
        if (fog > 1) fog = 0;
        map->material.shader->setInt("enableFog", fog);
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        seasonValue = 0.f;
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        seasonValue = 6.5f * (seasonalSpeed / 4.f);
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        seasonValue = 13.f * (seasonalSpeed / 4.f);
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        seasonValue = 19.5f * (seasonalSpeed / 4.f);

    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
        timeValue = 4.2f;
    if (key == GLFW_KEY_7 && action == GLFW_PRESS)
        timeValue = -0.5f;
    if (key == GLFW_KEY_8 && action == GLFW_PRESS)
        timeValue = 1.06f;
    if (key == GLFW_KEY_9 && action == GLFW_PRESS)
        timeValue = 2.65f;
}


// Object loader function, original: https://github.com/syoyo/tinyobjloader, changed to fit our needs
void objloadertest(Mesh* mesh, std::string path)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str());

    if (!err.empty())
    {
        LOG_WARN("%s", err.c_str());
    }
    if (!ret) exit(-1);

    for (size_t s = 0; s < shapes.size(); s++)
    {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            int fv = shapes[s].mesh.num_face_vertices[f];

            for (size_t v = 0; v < fv; v++)
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                // Vertices of the model
                mesh->vertices.push_back(glm::vec3(
                                                attrib.vertices[3 * idx.vertex_index + 0],
                                                attrib.vertices[3 * idx.vertex_index + 1],
                                                attrib.vertices[3 * idx.vertex_index + 2]));

                // Normals of the model
                mesh->normals.push_back(glm::vec3(
                                                attrib.normals[3 * idx.normal_index + 0],
                                                attrib.normals[3 * idx.normal_index + 1],
                                                attrib.normals[3 * idx.normal_index + 2]));

                // Texture coordinates of the model
                mesh->texCoord.push_back(glm::vec2(
                                                attrib.texcoords[2 * idx.texcoord_index + 0],
                                                attrib.texcoords[2 * idx.texcoord_index + 1]));

                /////////////////////////////////////////////////////////////////////////////////
                // For some reason this works, no idea why, but atleast it loads the entire model
                // Indices of the model
                /*
                mesh->indices.push_back(glm::uvec3(
                                                6 * idx.vertex_index + 0,
                                                6 * idx.vertex_index + 1,
                                                6 * idx.vertex_index + 2));
                mesh->indices.push_back(glm::uvec3(
                                                6 * idx.vertex_index + 3,
                                                6 * idx.vertex_index + 4,
                                                6 * idx.vertex_index + 5));
                /////////////////////////////////////////////////////////////////////////////////
                */
            }
            index_offset += fv;
        }
    }
}
