#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 model;

void main(){
    gl_Position = model * vec4(aPos.xy * 0.4f, aPos.z, 1.f);
    ourColor = aColor;
}