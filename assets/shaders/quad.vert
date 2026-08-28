#version 330 core

layout (location = 0) in vec3 vertex_position;

uniform mat4 view_projection_matrix;

out vec3 position;

void main() {
    position = vertex_position;
    gl_Position = view_projection_matrix * vec4(vertex_position, 1.0);
}