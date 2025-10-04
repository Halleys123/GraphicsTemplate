#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV; // texture coords
layout (location = 3) in vec3 aNormal;

uniform float time;
uniform float resolutionX;
uniform float resolutionY;

uniform float cameraX;
uniform float cameraZ;

out vec4 VertColor;
out vec2 UV;
out vec3 Normal;

void main()
{
    vec3 cameraPos = vec3(cameraX, 5.0, cameraZ);
    vec3 targetPos = vec3(0.0);
    vec3 up = vec3(0.0, 1.0, 0.0);

    // Build proper camera basis
    vec3 f = normalize(targetPos - cameraPos);        // forward
    vec3 r = normalize(cross(f, up));                 // right
    vec3 u = cross(r, f);                             // corrected up

    // Perspective
    float aspect = resolutionX / max(resolutionY, 1.0);
    float fov = radians(75.0);                       // try narrower first
    float fcoef = 1.0 / tan(fov * 0.5);
    float nearP = 0.1;
    float farP  = 200.0;

    mat4 P = mat4(
        fcoef / aspect, 0.0,   0.0,                                0.0,
        0.0,            fcoef, 0.0,                                0.0,
        0.0,            0.0,   (farP + nearP) / (nearP - farP),   -1.0,
        0.0,            0.0,   (2.0 * farP * nearP) / (nearP - farP), 0.0
    );

    // Column-major view matrix (rotation + translation in last column)
    mat4 V = mat4(
        vec4(r, 0.0),
        vec4(u, 0.0),
        vec4(-f, 0.0),
        vec4(-dot(r, cameraPos),
             -dot(u, cameraPos),
              dot(f, cameraPos),
              1.0)
    );

    gl_Position = P * V * vec4(aPos, 1.0);
    VertColor = aColor;
    UV = aUV;
    Normal = aNormal;
}