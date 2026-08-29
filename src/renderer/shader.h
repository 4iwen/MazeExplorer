#pragma once

#include "renderer_backend.h"

#include <string>

#include "glm/fwd.hpp"

class Shader {
public:
    Shader(
        const std::string &vertex_shader_source,
        const std::string &fragment_shader_source
    );

    ~Shader();

    Shader(const Shader &other) = delete;

    Shader &operator=(const Shader &other) = delete;

    void use() const;

    void set_mat4(const std::string &name, const glm::mat4 &value) const;

    void set_int(const std::string &name, int value) const;

    void set_vec4(const std::string& name,const glm::vec4& value) const;

private:
    void compile(
        const std::string &vertex_shader_source,
        const std::string &fragment_shader_source
    );

#ifdef RENDERER_OPENGL
    void check_for_compilation_errors(GLuint shader_id);

    void check_for_linking_errors() const;

    GLuint m_id;
#endif
};
