#version 150 core

uniform vec3 iResolution;

in vec2 position;

out vec2 fragCoord;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);

    fragCoord = (gl_Position.xy + vec2(1.0)) / vec2(2.0) * iResolution.xy;
}
