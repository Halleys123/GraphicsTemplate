#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV; // texture coords
layout (location = 3) in vec3 aNormal;

uniform float time;

out vec4 VertColor;
out vec2 UV;
out vec3 Normal;

void main()
{    
    mat4 translateMatrix = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        sin(time) * 0.5, cos(time) * 0.5, 0.0, 1.0
    );
    mat4 rotationOnXAxis = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, cos(time), -sin(time), 0.0,
        0.0, sin(time), cos(time), 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    
    mat4 rotationOnYAxis = mat4(
        cos(time), 0.0, sin(time), 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sin(time), 0.0, cos(time), 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    
    mat4 combinedRotation = rotationOnYAxis * rotationOnXAxis;

    gl_Position =  translateMatrix * combinedRotation * vec4(aPos.xyz / 2.0, 1.0);
    VertColor = aColor;
    UV = aUV;
    Normal = aNormal;
}