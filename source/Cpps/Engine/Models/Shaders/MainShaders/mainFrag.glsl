#version 410

in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;

out vec4 FragColor;

void main() {
    vec4 tex0Color = texture(texture0, texCoord);
    vec4 tex1Color = texture(texture1, texCoord);
    vec4 tex2Color = texture(texture2, texCoord);
    vec4 tex3Color = texture(texture3, texCoord);
    vec4 tex4Color = texture(texture4, texCoord);

    FragColor = tex1Color;
}
