#version 330 core

in vec2 texCoord;
in vec3 fragPos;
in mat3 TBN;
in vec3 flatNormal;

out vec4 FragColor;

uniform sampler2D albedo;
uniform sampler2D ao;
uniform sampler2D metallic;
uniform sampler2D normal;
uniform sampler2D roughness;
uniform bool textured;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;


uniform vec3 vAlbedo;
uniform float vAo;
uniform float vMetallic;
uniform float vRoughness;
uniform vec3 vNormal;

uniform vec3 viewPos;

#define nLights 4

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;

};

uniform PointLight pointLights[nLights];


vec3 calculateAmbient(float strength) {
    vec3 avg;
    for(int i = 0; i < nLights; ++i) {
        avg += pointLights[i].color;

    }

    return strength * (avg/nLights);
}



const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
void main()
{


    vec3 albedo = texture(albedo, texCoord).rgb;
    float roughness = texture(roughness, texCoord).r ;
    float metallic = texture(metallic, texCoord).r;
    float ao = texture(ao, texCoord).r;
    vec3 normal = texture(normal, texCoord).rgb;

    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(TBN * normal);


    // uncomment for untextured rendering

    if(textured) {
        //albedo = vec3(1,0,0);
        //roughness = .5;
        metallic = 0;
        ao = 1;
        //normal = flatNormal;
    }

    metallic = 0;
    ao=1;


    //metallic = 0;
    albedo = pow(albedo, vec3(2.2));



    vec3 N = normalize(normal);
    vec3 V = normalize(viewPos - fragPos);
    vec3 R = reflect(-V, N);





    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < nLights; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(pointLights[i].position - fragPos);
        vec3 H = normalize(V + L);
        float distance = length(pointLights[i].position - fragPos);
        float attenuation = pointLights[i].intensity / (distance * distance);
        vec3 radiance = pointLights[i].color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlickRoughness(clamp(dot(N, V), 0.0, 1.0), F0, roughness);

        vec3 nominator    = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0

        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }

    // ambient lighting (note that the next IBL tutorial will replace
    // this ambient lighting with environment lighting).
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;

    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color = ambient + Lo;

    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}
