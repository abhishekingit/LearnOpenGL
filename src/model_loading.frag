#version 410 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Material material; 
uniform Light light;
uniform bool hasTexture;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;



void main()
{   
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    
    float diff = max(dot(norm, lightDir), 0.0);

    float ambientStrength = 0.2;
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 result;

    if (hasTexture) {
        ambient = texture(texture_diffuse1, TexCoords).rgb * light.ambient;
        diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
        specular = (vec3(texture(texture_diffuse1, TexCoords)) * spec) * light.specular;
        result = ambient + diffuse;
    }
    else {
        ambient = material.ambient * light.ambient * ambientStrength;
        diffuse = light.diffuse * (diff * material.diffuse);
        specular = light.specular * (spec * material.specular);
        result = ambient + diffuse + specular;
    }
    FragColor = vec4(result, 1.0);
}