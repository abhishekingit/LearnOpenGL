#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 transform;
uniform mat4 viewM;
uniform mat4 projM;


void main() {
	gl_Position = projM * viewM * transform * vec4(aPos, 1.0);
}