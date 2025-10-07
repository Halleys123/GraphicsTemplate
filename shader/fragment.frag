#version 330 core

in vec4 VertColor;
in vec2 UV;
in vec3 Normal;
in vec3 fragPos;
out vec4 color;
uniform sampler2D textureSampler;

void main()
{
    vec3 lightPos= (vec3(-5,-5,-5));
    vec3 fragToLight =   (fragPos-lightPos);
    float cosine = max(dot(normalize(Normal), normalize(-fragToLight)), 0.0);
    float attenuation = 1.0 ;
    vec3 lightColor = vec3(1.0, 1.0, 1.0) * attenuation;

    color = VertColor * cosine * vec4(lightColor, 1.0) * texture(textureSampler, UV);
}