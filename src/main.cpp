//c++ Standard Library includes
#include <stdio.h>
#include <string.h>
#include <cmath>

//OpenGL includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;

const float speed = 2.f,
            rotationAngle = 120.f;
float lastTime = 0.f,
        deltaTime = 0.f,
        currentTime = 0.f;
bool direction = true;

float triOffset = 0.f,
        triMaxOffset = 0.6f,
        triIncretiment = 0.005f;

GLuint  VAO = 0, 
        VBO = 0, 
        EBO = 0, 
        shader = 0,
        uniformModel = 0;

static const char* vShader = 
"#version 330 core\n\
layout(location = 0) in vec3 aPos;\n\
layout(location = 1) in vec3 aColor;\n\
out vec3 OutColor;\n\
uniform mat4 model;\n\
void main(){\n\
    gl_Position = model * vec4(0.4f * aPos.x, 0.4f * aPos.y, 0.4f * aPos.z, 1.0);\n\
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
    uniformModel = glGetUniformLocation(shader, "model");
}
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
void MoveUpAndDown(GLFWwindow*window, glm::mat4& model, float speed, float deltaTime){
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        model = glm::translate(
                model, 
                glm::vec3(0.f, speed * deltaTime, 0.f)
        );
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        model = glm::translate(
                model, 
                glm::vec3(0.f, -speed * deltaTime, 0.f)
        );
    }
}
void Rotate(GLFWwindow*window, glm::mat4& model, float RotationAngle, float deltaTime){
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        model = glm::rotate(
                model,
                glm::radians(-RotationAngle) * deltaTime,
                glm::vec3(0.f, 0.f, 1.f)
        );
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        model = glm::rotate(
                model,
                glm::radians(RotationAngle) * deltaTime,
                glm::vec3(0.f, 0.f, 1.f)
        );
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
    lastTime = 0.f;
    while(!glfwWindowShouldClose(mainWindow)){
        glfwPollEvents();

        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        static glm::mat4 model(1.f);
        Rotate(mainWindow, model, rotationAngle, deltaTime);
        MoveUpAndDown(mainWindow, model, speed, deltaTime);
        /*model = glm::rotate(
            model,
            triOffset, 
            glm::vec3(0.f, 1.f, 0.f)
        );*/

        glUniformMatrix4fv(
            uniformModel, 
            1, 
            GL_FALSE,
            glm::value_ptr(model)
        );

        glBindVertexArray(VAO);

        processInput(mainWindow);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glUseProgram(0);
        
        glfwSwapBuffers(mainWindow);
    }
    return 0;
}