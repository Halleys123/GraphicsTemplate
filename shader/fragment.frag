#version 330 core

in vec4 VertColor;
in vec2 UV;
in vec3 Normal;
in vec3 fragPos;
out vec4 color;
uniform sampler2D textureSampler;

void main()
{
    color = VertColor * texture(textureSampler, UV);
}