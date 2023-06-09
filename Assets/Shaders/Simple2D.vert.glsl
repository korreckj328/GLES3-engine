#version 300 es

in vec2 vertPos;
out vec4 color;

const vec4 white = vec4(1.0);

void main() {
    color = white;
    gl_Position = vec4(vertPos, 0.0f, 1.0f);
}
