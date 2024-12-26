#version 410 core

out vec4 outputColor;
in vec3 fragColor;

void main() {
    outputColor = vec4(fragColor, 1.0);
}