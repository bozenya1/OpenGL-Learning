//inicialize c++ Standard Librarys
#include <stdio.h>
#include <string.h>

//inicialize OpenGL Librarys
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//inicialize Project Librarys
#include "main.h"

//Window size
const GLint WIDHT = 800, HEIGHT = 600;

//inicialize variables Shaders
GLuint VAO, VBO, shader = 0;

//Writing Vertex Shader
static const char* vShader = "                                          \n\
#version 330 core                                                       \n\
layout (location = 0) in vec3 pos;                                      \n\
void main(){                                                            \n\
    gl_Position = vec4(0.4f*pos.x, 0.4f*pos.y, 0.4f*pos.z, 1.f);        \n\
}                                                                       \n\
";

//Writing Fragment Shader
static const char* fShader = "                                          \n\
#version 330 core                                                       \n\
out vec4 color;                                                         \n\
void main(){                                                            \n\
    color = vec4(1.f, 0.f, 0.f, 1.0f);                                  \n\
}                                                                       \n\
";

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType){
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = {0};
    
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result){
        glGetShaderInfoLog(theShader, sizeof(eLog), nullptr, eLog);
        printf("Shader compiling failed:\n%s", eLog);
        return;
    }
    glAttachShader(theProgram, theShader);
}

//Compile Shaders and Link them to the program
void CompileShaders(){
    shader = glCreateProgram();

    //debbuging shader creation
    if(!shader){
        printf("Shader creation failed");
        return;
    }
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shader);

    //more debbuging
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
        printf("Shader linking failed:\n%s", eLog);
        return;
    }
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
        printf("Shader validating failed:\n%s", eLog);
        return;
    }
}
//Function crates Triangle
void CreateTriangle(){
    //Position vertex for triangle
    GLfloat vertices[] = {
        -1.f, -1.f, 0.f,
        1.f, -1.f, 0.f,
        0.f, 1.f, 0.f
    };
    //Create Object and assign an ID
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Create Object and assign data to VRAM
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT, 
        GL_FALSE, 
        3 * sizeof(GLfloat), 
        (void*)0);
    glEnableVertexAttribArray(0);
    //closing the buffer and vertex array
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

//main function
int main(){
    //inicialize platform
    Platform_Init();

    //inicialize glfw
    if(glfwInit() != GLFW_TRUE){
        printf("glfw Inicialize Failed");
        glfwTerminate();
        return 1;
    }
    //inicialize OpenGL version
    OpenGL_Version_Init();

    //create window
    GLFWwindow *mainWindow = glfwCreateWindow(WIDHT, HEIGHT, "Title", nullptr, nullptr);
    if(!mainWindow){
        printf("Window doesnt create... Failed");
        glfwTerminate();
        return 1;
    }
    //get window size
    int weightBuffer, hightBuffer;
    glfwGetFramebufferSize(mainWindow, &weightBuffer, &hightBuffer);

    //create OpenGL context
    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;

    //inicialize glew
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
    //Inicialize Shaders
    CreateTriangle();
    CompileShaders();

    //main loop
    while(!glfwWindowShouldClose(mainWindow)){
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Inicialize Shaders in the main loop
        glUseProgram(shader);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }
    //end of main loop
    return 0;
}
