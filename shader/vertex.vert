#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV; // texture coords
layout (location = 3) in vec3 aNormal;

uniform float time;
uniform float resolutionX;
uniform float resolutionY;

out vec4 VertColor;
out vec2 UV;
out vec3 Normal;

void main()
{
    float aspect = resolutionX / max(resolutionY, 1.0);
    float fov = radians(120.0);
    float f = 1.0 / tan(fov * 0.5);
    float nearP = 0.1;
    float farP = 200.0;

    // Standard OpenGL RH perspective matrix (column major)
    mat4 P = mat4(
        f / aspect, 0.0, 0.0, 0.0,
        0.0, f, 0.0, 0.0,
        0.0, 0.0, (farP + nearP) / (nearP - farP), -1.0,
        0.0, 0.0, (2.0 * farP * nearP) / (nearP - farP), 0.0
    );

    // gl_Position = vec4(aPos, 1.0);
    gl_Position = P * vec4(aPos, 1.0);
    VertColor = aColor;
    UV = aUV;
    Normal = aNormal;
}