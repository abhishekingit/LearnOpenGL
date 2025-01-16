#version 410 core

out vec4 outputColor;
in vec3 fragColor;
in vec2 TexCoords;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D ourTexture3;
uniform sampler2D ourTexture4;


uniform float weight1;
uniform float weight2;
uniform float weight3;

uniform vec4 finalColor;

uniform bool useStaticTexture;

void main() {

    vec4 color1 = texture(ourTexture1, TexCoords);
    vec4 color2 = texture(ourTexture2, TexCoords);   
    vec4 color3 = texture(ourTexture3, TexCoords);
    vec4 color4 = texture(ourTexture4, TexCoords);


    if(useStaticTexture) {
        outputColor = color4;
    }
    else {
        float totalWeight = weight1 + weight2 + weight3;
        vec4 blendedColor = (weight1 * color1 + weight2 * color2 + weight3 * color3) / totalWeight;
        outputColor = blendedColor * finalColor;    
    }    
    
}