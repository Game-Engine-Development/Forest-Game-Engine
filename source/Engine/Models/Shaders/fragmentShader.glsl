#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D sampler;

void main()
{
    vec3 lightColor = vec3(1, 0, 0);
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;
    FragColor = vec4(ambient, 1.0) * texture(sampler, texCoord);;
}