#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
out vec3 vNormal;
out vec2 TexCoord;
uniform mat4 MVP;
uniform mat4 MV;
uniform vec3 lightPos;
void main(){
    gl_Position = MVP * vec4(aPos, 1.0);
    mat4 normalMtx = transpose(inverse(MV));
    vNormal = (normalMtx * vec4(aNormal, 0.0)).xyz;
    TexCoord = aTexCoord;
}