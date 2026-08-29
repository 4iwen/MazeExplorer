#version 330 core

layout (location = 0) in vec3 vertex_position;

uniform mat4 view_projection_matrix;

void main() {
    gl_Position = view_projection_matrix * vec4(vertex_position, 1.0);
}