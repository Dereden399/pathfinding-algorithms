#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;

void main() {
    FragColor = Color;
}