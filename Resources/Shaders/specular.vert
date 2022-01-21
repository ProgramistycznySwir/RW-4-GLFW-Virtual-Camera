#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
out vec3 vNormal;
out vec3 vLight;
out vec3 vEye;
uniform mat4 MVP;
uniform mat4 MV;
uniform vec3 lightPos;
void main(){ 
    gl_Position = MVP * vec4(aPos, 1.0);
    mat4 normalMtx = transpose(inverse(MV));
    vNormal = (normalMtx * vec4(aNormal, 0.0)).xyz;
    vec4 vPos = MV * vec4(aPos, 1.0);
    vec4 vLightPos = MV * vec4(lightPos, 1.0);
    vLight = vLightPos.xyz - vPos.xyz;
    vEye = -vPos.xyz;
}