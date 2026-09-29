#version 330 core
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

struct Material {
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    float shininess;
};
uniform Material material;

#define DIRECTIONAL 1
#define POINT       2
#define SPOT        3
struct Light {
    int type;
    vec4 position;
    vec4 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float a;
    float b;
    float c;
    float cutoff_inner;
    float cutoff_outer;
    bool enabled;
};
#define MAX_LIGHTS 16
uniform Light lights[MAX_LIGHTS];

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
  
out vec4 FragColor;

vec3 handleDirectional(int i) {
    vec3 ambient = material.ambient_color * lights[i].ambient;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-lights[i].direction.xyz);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lights[i].diffuse * max(diff, 0.1);
    diffuse *= vec3(texture(texture_diffuse1, TexCoords));

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lights[i].specular * max(spec, 0.1);
    specular *= vec3(texture(texture_specular1, TexCoords));

    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 handlePoint(int i) {
    float distance    = length(lights[i].position.xyz - FragPos);
    float attenuation = 1.0 / (lights[i].c + lights[i].b * distance + 
                lights[i].a * (distance * distance)); 
    vec3 ambient = material.ambient_color * lights[i].ambient;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lights[i].position.xyz - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lights[i].diffuse * max(diff, 0.1);
    diffuse *= vec3(texture(texture_diffuse1, TexCoords));

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lights[i].specular * max(spec, 0.1);
    specular *= vec3(texture(texture_specular1, TexCoords));

    vec3 result = ambient + diffuse + specular;
    result *= attenuation;
    return result;
}

vec3 handleSpot(int i) {
    float distance    = length(lights[i].position.xyz - FragPos);
    float attenuation = 1.0 / (lights[i].c + lights[i].b * distance + 
                lights[i].a * (distance * distance)); 
    vec3 ambient = material.ambient_color * lights[i].ambient;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lights[i].position.xyz - FragPos);
    float theta = dot(lightDir, normalize(-lights[i].direction.xyz));
    if (theta <= lights[i].cutoff_outer)
        return ambient * attenuation;

    float epsilon   = lights[i].cutoff_inner - lights[i].cutoff_outer;
    float intensity = smoothstep(0.0, 1.0, (theta - lights[i].cutoff_outer) / epsilon);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lights[i].diffuse * max(diff, 0.1) * intensity;
    diffuse *= vec3(texture(texture_diffuse1, TexCoords));

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lights[i].specular * max(spec, 0.1) * intensity;
    specular *= vec3(texture(texture_specular1, TexCoords));

    vec3 result = ambient + diffuse + specular;
    result *= attenuation;
    return result;
}

void main()
{
    vec3 result = vec3(0.0);
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        if (!lights[i].enabled)
            continue;
        if (lights[i].type == DIRECTIONAL)
            result += handleDirectional(i);
        else if (lights[i].type == POINT)
            result += handlePoint(i);
        else if (lights[i].type == SPOT)
            result += handleSpot(i);
    }
    FragColor = vec4(result, 1.0);
}

