#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 3) in vec2 vertex_uv;

uniform mat4 view_projection_matrix;

out vec2 uv;

void main() {
    uv = vertex_uv;

    gl_Position = view_projection_matrix * vec4(vertex_position, 1.0);
}