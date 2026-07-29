#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define OpenGL_Init() do {\
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);\
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);\
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);\
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);\
} while (0);