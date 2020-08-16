#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform bool gammaOn;
uniform float exposure;

void main()
{
    const float gamma = 2.2;
    vec3 result = texture(hdrBuffer, TexCoords).rgb;
    if(hdr)
    {
        // reinhard
        // vec3 result = hdrColor / (hdrColor + vec3(1.0));
        // exposure
        result = vec3(1.0) - exp(-result * exposure);
        // also gamma correct while we're at it
    }
    if(gammaOn)
    {
        result = pow(result, vec3(1.0 / gamma));
    }
    FragColor = vec4(result, 1.0);


}