#version 300 es
#ifdef GL_ES
precision highp float;
#endif

in vec4 color;
out vec4 fragColor;

void main() {
    fragColor = color;
}
