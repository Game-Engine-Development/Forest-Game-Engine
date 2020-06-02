#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec3 FragPos;
in mat3 TBN;

in vec3 T;
in vec3 B;
in vec3 N;


out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 viewPos;

mat3 cotangent_frame( vec3 N, vec3 p, vec2 uv )
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );

    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

    // construct a scale-invariant frame
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

void main()
{
    vec2 tiledTexCoord = texCoord * 40;

    vec3 norm = texture(texture4, tiledTexCoord).rgb;
    //vec3 norm = vec3(.5, .5, 1);


    // transform normal vector to range [-1,1]
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(cotangent_frame(normal, FragPos, tiledTexCoord) * norm);

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
    vec4 backgroundTexture = texture(texture1, tiledTexCoord) * backgroundTextureStrength;
    vec4 rColor = texture(texture2, tiledTexCoord) * color.r;
    vec4 gColor = texture(texture3, tiledTexCoord) * color.g;
    vec4 bColor = texture(texture4, tiledTexCoord) * color.b;
    vec4 totalColor = backgroundTexture + rColor + gColor + bColor;

    vec4 result = vec4((ambient + diffuse + specular), 1.0) * totalColor;

    result = vec4(B, 1);
    FragColor = result;
}