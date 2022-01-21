#version 330 core
in vec3 vNormal;
in vec2 TexCoord;
uniform sampler2D Texture;
const vec4 Ka = vec4(0.1, 0.1, 0.1, 1.0);
const vec4 Il = vec4(1.0, 1.0, 1.0, 1.0);
void main() {
    vec4 tex = texture(Texture, TexCoord);
    gl_FragColor = tex;
}