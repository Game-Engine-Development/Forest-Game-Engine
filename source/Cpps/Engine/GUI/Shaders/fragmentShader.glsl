#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform bool isTransparent;
uniform bool backgroundImg;

void main() {
    vec4 result = vec4(texture(texture0, texCoord));
    if(result.a < 0.4) {
        if(isTransparent) {
            discard;
        } else if(backgroundImg) {
            result = vec4(texture(texture1, texCoord));
            if(result.a < 0.4) {
                discard;
            }
        }
    }

    FragColor = result;
}
