#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture0;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 viewPos;

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrength = 0.8;
    vec3 ambient = ambientStrength * lightColor;

    float specularStrength = 0.2;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    /*vec4 color;
    if(FragPos.y < 0.3) {
        color = vec4(0, 0, 255, 0);
    }
    else if(FragPos.y > 30) {
        color = vec4(245, 245, 245, 255);
    }
    else {
        color = texture(texture0, texCoord);
    }*/

    vec4 color = texture(texture0, texCoord);

    vec4 result = vec4((ambient + diffuse + specular), 1.0) * color;

    FragColor = result;
}