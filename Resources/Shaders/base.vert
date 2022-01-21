#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 MVP;
attribute vec3 vCol;
varying vec3 color;

void main(){
    gl_Position = MVP * vec4(aPos, 1.0);
    color = vec3(1);
}