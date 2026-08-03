#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "main.h"

const GLint WIDHT = 800, HEIGHT = 600;

int main(){
    Platform_Init();
    if(glfwInit() != GLFW_TRUE){
        printf("glfw Inicialize Failed");
        glfwTerminate();
        return 1;
    }
    OpenGL_Version_Init();
    GLFWwindow *mainWindow = glfwCreateWindow(WIDHT, HEIGHT, "Title", nullptr, nullptr);
    if(!mainWindow){
        printf("Window doesnt create... Failed");
        glfwTerminate();
        return 1;
    }
    int weightBuffer, hightBuffer;
    glfwGetFramebufferSize(mainWindow, &weightBuffer, &hightBuffer);

    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK){
        printf("Glew inicializating failed");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    glViewport(
        0,
        0,
        weightBuffer,
        hightBuffer
    );
    while(!glfwWindowShouldClose(mainWindow)){
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    
        glfwSwapBuffers(mainWindow);
    }
    return 0;
}
