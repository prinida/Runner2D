#include "OpenGLManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>

OpenGLManager* openGLManager;

void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight)
{
    if ((float)newHeight > 1e-7)
    {
        openGLManager->RecalculateProjectionMatrix((float)newWidth / (float)newHeight);
        glViewport(0, 0, newWidth, newHeight);
    }
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        openGLManager->GetPlayer()->LeftMove();
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        openGLManager->GetPlayer()->RightMove();
}

int main(int argc, char** argv)
{
    if (!glfwInit()) { exit(EXIT_FAILURE); }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(600, 600, "Runner", NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(3);

    glfwSetWindowSizeCallback(window, window_reshape_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { exit(EXIT_FAILURE); }

    openGLManager = new OpenGLManager(argv[0]);

    openGLManager->Init(window);

    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window);

        openGLManager->Display(window, glfwGetTime());

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    exit(EXIT_SUCCESS);
}