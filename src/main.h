#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

inline void OpenGL_Version_Init() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #if defined(__APPLE__)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
}
inline void Platform_Init() {
    #if defined(_WIN32)
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WIN32);
    #elif defined(__linux__)
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    #elif defined(__APPLE__)
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_COCOA);
    #endif
}