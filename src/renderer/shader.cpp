#include "shader.h"

#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(
    const std::string &vertex_shader_source,
    const std::string &fragment_shader_source
) : m_id(0) {
    compile(
        vertex_shader_source,
        fragment_shader_source
    );
}

#ifdef RENDERER_OPENGL
Shader::~Shader() {
    glDeleteProgram(m_id);
}

void Shader::use() const {
    glUseProgram(m_id);
}

void Shader::set_mat4(const std::string &name, const glm::mat4 &value) const {
    GLint location = glGetUniformLocation(m_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
}

void Shader::compile(
    const std::string &vertex_shader_source,
    const std::string &fragment_shader_source
) {
    GLuint vertex_id = glCreateShader(GL_VERTEX_SHADER);
    const char *vertex_shader_c_str = vertex_shader_source.c_str();
    glShaderSource(vertex_id, 1, &vertex_shader_c_str, nullptr);
    glCompileShader(vertex_id);
    check_for_compilation_errors(vertex_id);

    GLuint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragment_shader_c_str = fragment_shader_source.c_str();
    glShaderSource(fragment_id, 1, &fragment_shader_c_str, nullptr);
    glCompileShader(fragment_id);
    check_for_compilation_errors(fragment_id);

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex_id);
    glAttachShader(m_id, fragment_id);
    glLinkProgram(m_id);
    check_for_linking_errors();
    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
}

void Shader::check_for_compilation_errors(GLuint shader_id) {
    GLint success;

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        constexpr size_t log_buffer_size = 512;
        char log_data[log_buffer_size];
        glGetShaderInfoLog(shader_id, log_buffer_size, nullptr, log_data);
        throw std::runtime_error(log_data);
    }
}

void Shader::check_for_linking_errors() const {
    GLint success;

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        constexpr size_t log_buffer_size = 512;
        char log_data[log_buffer_size];
        glGetProgramInfoLog(m_id, log_buffer_size, nullptr, log_data);
        throw std::runtime_error(log_data);
    }
}
#endif
