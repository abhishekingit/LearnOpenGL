#version 410 core

out vec4 outputColor;
in vec3 fragColor;
in vec2 TexCoords;

uniform sampler2D ourTexture;

void main() {
    outputColor = texture(ourTexture, TexCoords);
}