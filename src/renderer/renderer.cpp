#include "renderer.h"

#include "platform/platform.h"

#include <stdexcept>
#include <glad/gl.h>

const char *vertex_shader_source = R"(
#version 330 core

layout (location = 0) in vec3 vertex_position;

out vec3 position;

void main() {
    position = vertex_position;
    gl_Position = vec4(vertex_position, 1.0);
}
)";

const char *fragment_shader_source = R"(
#version 330 core

in vec3 position;

out vec4 fragment_color;

void main() {
    fragment_color = vec4(position, 1.0);
}
)";

float vertices[] = {
    0.5f,  0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f,
   -0.5f,  0.5f, 0.0f
};
unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

Renderer::Renderer() {
    // Load GL functions
    if (!gladLoadGL(glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD.");
    }

    // Create vertex shader
    m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(m_vertex_shader);

    // Check for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(m_vertex_shader, 512, nullptr, infoLog);
        throw std::runtime_error(infoLog);
    }

    // Create fragment shader
    m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(m_fragment_shader);

    // Check for compilation errors
    glGetShaderiv(m_fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(m_fragment_shader, 512, nullptr, infoLog);
        throw std::runtime_error(infoLog);
    }

    // Create shader program
    m_shader_program = glCreateProgram();
    glAttachShader(m_shader_program, m_vertex_shader);
    glAttachShader(m_shader_program, m_fragment_shader);
    glLinkProgram(m_shader_program);

    // Check for linking errors
    glGetProgramiv(m_shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_shader_program, 512, nullptr, infoLog);
        throw std::runtime_error(infoLog);
    }
    glDeleteShader(m_vertex_shader);
    glDeleteShader(m_fragment_shader);

    // Vertex data
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteProgram(m_shader_program);
}

void Renderer::render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_shader_program);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    // glBindVertexArray(0);
}
