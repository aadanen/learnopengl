#version 330 core
struct Material {
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    sampler2D diffuse_map;
    sampler2D specular_map;
    float shininess;
};

#define DIRECTIONAL 1
#define POINT       2
#define SPOT        3
struct Light {
    int type;
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;
in vec2 TexCoords;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform bool useTextures;

void main()
{
    vec3 ambient = material.ambient_color * lightColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse;
    if (useTextures)
        diffuse = lightColor * diff * vec3(texture(material.diffuse_map, TexCoords));
    else
        diffuse = lightColor * diff * material.diffuse_color;

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular;
    if (useTextures)
        specular = lightColor * spec * vec3(texture(material.specular_map, TexCoords));
    else
        specular = lightColor * spec * material.specular_color;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}

