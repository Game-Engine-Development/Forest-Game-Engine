#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;

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

    vec4 color = texture(texture0, texCoord);
    float backgroundTextureStrength = 1 - (color.r + color.g + color.b);
    vec2 tiledTexCoord = texCoord * 40;
    vec4 backgroundTexture = texture(texture1, tiledTexCoord) * backgroundTextureStrength;
    vec4 rColor = texture(texture2, tiledTexCoord) * color.r;
    vec4 gColor = texture(texture3, tiledTexCoord) * color.g;
    vec4 bColor = texture(texture4, tiledTexCoord) * color.b;
    vec4 totalColor = backgroundTexture + rColor + gColor + bColor;

    vec4 result = vec4((ambient + diffuse + specular), 1.0) * totalColor;

    FragColor = result;
}