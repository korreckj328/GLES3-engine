#version 120

in vec2 vertPos;
out vec4 color;

const vec4 white = vec4(1.0)

void main() {
    color = white;
    gl_position = vec4(vertPos, 0.0, 1.0)
}