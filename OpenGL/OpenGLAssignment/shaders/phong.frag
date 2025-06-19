#version 330 core
struct Material {
    sampler2D texture1;
    sampler2D texture2;
    float blendFactor;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int type; // 0 = directional, 1 = point, 2 = spot
};

#define MAX_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform Material material;
uniform Light lights[MAX_LIGHTS];
uniform int numLights;
uniform vec3 viewPos;
uniform sampler2D shadowMap;

// SHADOW FONKSİYONU
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Shadow map sınırında mıyız?
    if(projCoords.z > 1.0) return 0.0;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    // bias (self-shadow engellemek için)
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec4 texColor1 = texture(material.texture1, TexCoords);
    vec4 texColor2 = texture(material.texture2, TexCoords);
    vec4 baseColor = mix(texColor1, texColor2, material.blendFactor);

    vec3 result = vec3(0.0f);
    for (int i = 0; i < numLights; ++i) {
        vec3 lightDir;
        if (lights[i].type == 0) // directional
            lightDir = normalize(-lights[i].direction);
        else // point or spot
            lightDir = normalize(lights[i].position - FragPos);

        // Ambient
        vec3 ambient = lights[i].ambient * material.ambient;

        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = lights[i].diffuse * (diff * material.diffuse);

        // Specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = lights[i].specular * (spec * material.specular);

        // SADECE directional ışık için shadow uygulanacak (index 0)
        float shadow = 0.0;
        if(i == 0) shadow = ShadowCalculation(FragPosLightSpace);

        result += (ambient + (1.0 - shadow) * (diffuse + specular));
    }

    FragColor = vec4(result, 1.0) * baseColor;
}