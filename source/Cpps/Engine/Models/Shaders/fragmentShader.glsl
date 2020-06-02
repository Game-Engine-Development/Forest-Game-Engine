#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform bool gammaOn;
uniform float exposure;

vec3 tonemap() {
    vec3 result = texture(hdrBuffer, TexCoords).rgb;
    return vec3(1.0) - exp(-result * exposure);
}

vec3 gammaCorrect(vec3 uncorrected, float gamma){
            return pow(uncorrected, vec3(1.0 / gamma));
    }

void main()
{
    const float gamma = 2.2;

    vec3 color;

    if(hdr) { color = tonemap(); }
    if(gammaOn) { color = gammaCorrect(color, gamma); }

    FragColor = vec4(color, 1.0);


}