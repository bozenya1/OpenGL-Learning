//c++ Standard Library includes
#include <stdio.h>
#include <string.h>
#include <cmath>

//OpenGL includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>s

const GLint WIDTH = 800, HEIGHT = 600;

bool direction = true;

float triOffset = 0.f,
        triMaxOffset = 0.6f,
        triIncretiment = 0.0005f;

GLuint  VAO = 0, 
        VBO = 0, 
        EBO = 0, 
        shader = 0,
        uniformXMove = 0;

static const char* vShader = 
"#version 330 core\n\
layout(location = 0) in vec3 aPos;\n\
layout(location = 1) in vec3 aColor;\n\
out vec3 OutColor;\n\
uniform float XMove;\n\
void main(){\n\
    gl_Position = vec4(0.4f * aPos.x + XMove, 0.4f * aPos.y, 0.4f * aPos.z, 1.0);\n\
    OutColor = aColor;\n\
}\n" ;

static const char* fShader = 
"#version 330 core\n\
out vec4 FragColor;\n\
in vec3 OutColor;\n\
void main(){\n\
    FragColor = vec4(OutColor, 1.0f);\n\
}\n";
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
        printf("Error compiling the shader:\n%s", eLog);
        return;
    }
    glAttachShader(theProgram, theShader);
}
void CreateTriangle(){
    GLfloat vertices[] = {
        -1.f, -1.f, 0.f,    1.f, 0.f, 0.f,
        1.f, -1.f, 0.f,     0.f, 1.f, 0.f,
        1.f, 1.f, 0.f,      0.f, 0.f, 1.f,
        -1.f, 1.f, 0.f,     1.f, 1.f, 0.f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, 
        3, 
        GL_FLOAT, 
        GL_FALSE, 
        6 * sizeof(GLfloat), 
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 
        3, 
        GL_FLOAT, 
        GL_FALSE, 
        6 * sizeof(GLfloat), 
        (void*)(3 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void CompileShader(){
    shader = glCreateProgram();
    if(!shader){
        printf("Shader creation failed!");
        return;
    }
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
        printf("Error linking program:\n%s", eLog);
        return;
    }
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if(!result){
        glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
        printf("Error validating program:\n%s", eLog);
        return;
    }
    uniformXMove = glGetUniformLocation(shader, "XMove");
}
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
int main(){
    if(glfwInit() != GLFW_TRUE){
        printf("GLFW initialization failed!");
        glfwTerminate();
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", nullptr, nullptr);
    if(!mainWindow){
        printf("Failed to create GLFW window!");
        glfwTerminate();
        return 1;
    }
    GLint bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK){
        printf("GLEW initialization failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    glViewport(
        0, 
        0, 
        bufferWidth, 
        bufferHeight
    );
    CompileShader();
    CreateTriangle();
    while(!glfwWindowShouldClose(mainWindow)){
        glfwPollEvents();

        if(direction){
            triOffset += triIncretiment;

        } else {
            triOffset -= triIncretiment;
        }

        if(std::abs(triOffset) >= triMaxOffset){
            direction = !direction;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glUniform1f(uniformXMove, triOffset);

        glBindVertexArray(VAO);

        processInput(mainWindow);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glUseProgram(0);
        
        glfwSwapBuffers(mainWindow);
    }
    return 0;
}