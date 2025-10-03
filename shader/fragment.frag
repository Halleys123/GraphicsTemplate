#version 330 core

in vec4 VertColor;
in vec2 UV;
in vec3 Normal;

out vec4 color;
uniform sampler2D textureSampler;

void main()
{
    color = texture2D(textureSampler, UV) * VertColor;
}