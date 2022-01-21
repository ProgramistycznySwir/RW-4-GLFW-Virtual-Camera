#version 330 core

in vec3 vNormal;
in vec3 vLight;
in vec2 TexCoord;

uniform sampler2D Texture;

const vec4 Ka = vec4(0.1, 0.1, 0.1, 1.0);
const vec4 Il = vec4(1.0, 1.0, 1.0, 1.0);


void main(){

	vec3 vNormal_norm = normalize(vNormal);
	vec3 vLight_norm = normalize(vLight);

	float diff = clamp(dot(vNormal_norm, vLight_norm), 0, 1);
	vec4 tex = texture(Texture, TexCoord);
	gl_FragColor = tex *(Ka + diff *Il); // dla jasnej tekstury -> 	gl_FragColor = tex +(Ka + diff *Il);
}