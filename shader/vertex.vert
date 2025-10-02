#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV; // texture coords
layout (location = 3) in vec3 aNormal;

out vec4 VertColor;
out vec2 UV;
out vec3 Normal;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    VertColor = aColor;
    UV = aUV;
    Normal = aNormal;
}