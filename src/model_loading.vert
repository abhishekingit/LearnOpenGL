#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 transform;
uniform mat4 viewM;
uniform mat4 projM;

void main()
{
   
    FragPos = vec3(transform * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(transform))) * aNormal;
    TexCoords = aTexCoords; 
    gl_Position = projM * viewM * vec4(FragPos, 1.0);
}