#version 330 core

in vec2 texCoord;
//in vec3 normal;
in vec3 fragPos;
in mat3 TBN;

out vec4 FragColor;
//color
uniform sampler2D texture0;
uniform sampler2D texture1;
//normal map
uniform sampler2D texture2;
//specular map
uniform sampler2D texture3;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 viewPos;

void main()
{

    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(texture2, texCoord).rgb;
    // transform normal vector to range [-1,1]  
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(TBN * normal);

    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrength = .5;
    vec3 ambient = ambientStrength * lightColor;

    //value is a scaling factor for the specular map
    float specularStrength = texture(texture3, texCoord).r * 4;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), 5);
    vec3 specular = specularStrength * spec * lightColor;

    vec4 result = vec4((ambient + diffuse + specular), 1.0) * texture(texture0, texCoord);

    FragColor = result;
}