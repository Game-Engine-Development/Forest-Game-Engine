#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D texture0;

void main() {
    //vec4 result = vec4(texture(texture0, texCoord), 1.0);
    vec4 result = vec4(1.0f, 0.5f, 0.2f, 1.0f);

    FragColor = result;
}
