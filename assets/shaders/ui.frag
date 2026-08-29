#version 330 core

in vec2 uv;

uniform sampler2D texture_sampler;

out vec4 fragment_color;

void main() {
    fragment_color = texture(texture_sampler, uv);
}