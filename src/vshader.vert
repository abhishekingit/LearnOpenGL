#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoords;

out vec3 fragColor;
out vec2 TexCoords;

uniform mat4 transform;

void main() {
	gl_Position = transform * vec4(aPos, 1.0);
	fragColor = aColor;
	TexCoords = aTexCoords;
}