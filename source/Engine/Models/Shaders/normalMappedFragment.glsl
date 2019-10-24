#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;
//color
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 viewPos;

void main()
{
    // obtain normal from normal map in range [0,1]
    vec3 norm = texture(texture2, texCoord).rgb;
    // transform normal vector to range [-1,1]
    norm = normalize(norm * 2.0 - 1.0);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    float specularStrength = 0.3;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec4 result = vec4((ambient + diffuse + specular), 1.0) * mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.0);

    FragColor = result;
}