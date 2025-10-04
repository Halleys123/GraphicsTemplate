#version 330 core

// fragment shader

uniform vec2 iResolution; // viewport resolution (in pixels)
uniform float iTime; // shader playback time (in seconds)

out vec4 fragColor;

void main() {
    float x = gl_FragCoord.x / iResolution.x;
    float y = gl_FragCoord.y / iResolution.y;

    float radius = 0.5;

    float distanceFromCenter = length(vec2(sin(iTime) * x - 0.5, y - 0.5)) < radius ? 1.0 : 0.0;

    fragColor = vec4(distanceFromCenter, distanceFromCenter, 0.0, 1.0);
}