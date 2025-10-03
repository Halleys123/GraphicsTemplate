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
    float a = resolutionX / resolutionY;
    float fov = 120.0;
    float f = 1.0 / tan(radians(fov / 2.0));

    float near = 0.2;
    float far = 100.0;

    float lambda = (far) / (far - near);
    mat4 perspectiveMatrix = mat4(
        f / a, 0.0, 0.0, 0.0,
        0.0, f, 0.0, 0.0,
        0.0, 0.0, lambda, 1.0,
        0.0, 0.0, -lambda * near, 0.0
    );

    // mat4 translateMatrix = mat4(
    //     1.0, 0.0, 0.0, 0.0,
    //     0.0, 1.0, 0.0, 0.0,
    //     0.0, 0.0, 1.0, 0.0,
    //     sin(time) * 0.5, cos(time) * 0.5, 0.0, 1.0
    // );

    mat4 rotationOnZAxis = mat4(
        cos(time), -sin(time), 0.0, 0.0,
        sin(time), cos(time), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    mat4 combinedRotation = rotationOnZAxis;

    gl_Position =  perspectiveMatrix * combinedRotation * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    VertColor = aColor;
    UV = aUV;
    Normal = aNormal;
}