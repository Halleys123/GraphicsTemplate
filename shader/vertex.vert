#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

uniform float time;
uniform float resolutionX;
uniform float resolutionY;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 VertColor;
out vec2 UV;
out vec3 Normal;
out vec3 fragPos;

void main()
{

    gl_Position = projection * model * vec4(aPos, 1.0);

    VertColor = vec4(aColor.rgb, 1.0);
    UV = aUV;
    Normal = aNormal;
    fragPos = aPos;
}