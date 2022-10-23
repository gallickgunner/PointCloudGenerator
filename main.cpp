// testopengl.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#define GLFW_DLL

#include <iostream>
#include <vector>

#include "Scene.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ShaderUtils.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorPosCallback(GLFWwindow* window, double new_cursor_x, double new_cursor_y);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers);
void mouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset);

bool mouse_button_pressed = false;
float old_cursor_x = 0;
float old_cursor_y = 0;

int main()

{
    if (!glfwInit())
        throw std::runtime_error("GLFW failed to initialize.");

    GLFWwindow* window = NULL;

    //Window Hints
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window = glfwCreateWindow(1280, 720, "test", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Window creation failed..");
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Couldn't Initialize GLAD..");

    //If possible download glad loader with opengl version 3.3 as this will make things a little easier (fewer opengl calls) - https://glad.dav1d.de/
    // Refer to the lecture video for details
    if (!GLAD_GL_VERSION_3_3)
        throw std::runtime_error("OpenGL version 3.3 not supported");

    glGetError();
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);
    glfwSwapInterval(1);
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    glEnable(GL_PROGRAM_POINT_SIZE);

    //set Key Press Callback
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    Scene main_scene;
    main_scene.createShaderProgram();
    main_scene.createScene();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        main_scene.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

void cursorPosCallback(GLFWwindow* window, double new_cursor_x, double new_cursor_y)
{
    if(mouse_button_pressed)
    {
        float delta_x, delta_y;
        float zenith = 0.06, azimuth = 0.06;

        delta_x = new_cursor_x - old_cursor_x;
        delta_y = new_cursor_y - old_cursor_y;

        if(delta_x < 1 && delta_x > -1)
            azimuth = 0;
        else if (delta_x > 0)
            azimuth *= -1.0;

        if(delta_y < 1 && delta_y > -1)
            zenith = 0;
        else if(delta_y > 0)
            zenith *= -1.0;

        Camera::main_cam->setOrientation(0, zenith, azimuth);

        old_cursor_x = new_cursor_x;
        old_cursor_y = new_cursor_y;
    }
    else
    {
        old_cursor_x = new_cursor_x;
        old_cursor_y = new_cursor_y;
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        mouse_button_pressed = true;
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        mouse_button_pressed = false;
    }
}

void mouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
    Camera::main_cam->setOrientation(y_offset, 0, 0);
}
