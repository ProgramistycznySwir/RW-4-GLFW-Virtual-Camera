#version 330 core
in vec3 vNormal;
in vec3 vLight;
in vec3 vEye;
const vec4 Ka = vec4(0.2, 0.0, 0.0, 1.0);
const vec4 Ks = vec4(1.0, 0.0, 0.0, 1.0);
const vec4 Il = vec4(1.0, 1.0, 0.0, 1.0);
void main() {
    vec3 vNormal_norm = normalize(vNormal);
    vec3 vLight_norm = normalize(vLight);
    vec3 vEye_norm = normalize(vEye);
    float b = step(0, dot(vLight_norm, vNormal_norm)); //reflect(I, N) = I -2.0 * dot(N, I) * N;
    vec3 vReflect_norm = reflect(-vLight_norm, vNormal_norm);
    float spec = pow(clamp(dot(vReflect_norm, vEye_norm), 0, 1), 8); //     float spec = pow(clamp(dot(vReflect_norm, vEye_norm), 0, 1), 64); 
    //model Blinna-Phonga
    //vec3 vHalfWay_norm = normalize(vLight + vEye);
    //float spec = pow(clamp(dot(vHalfWay_norm, vNormal_norm), 0, 1), 8);
    gl_FragColor = Ka + b * Ks * spec * Il; // gl_FragColor = Ka + bKs spec *Il;
}