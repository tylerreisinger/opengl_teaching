#ifndef SHADER_H_
#define SHADER_H_

#include "GlHeaders.h"

#include <string>
#include <memory>
#include <vector>

enum class ShaderKind: GLuint {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
};

class Shader {
public:
    Shader(ShaderKind kind);
    ~Shader();

    Shader(const Shader& other) = delete;
    Shader(Shader&& other) noexcept = delete;
    Shader& operator =(const Shader& other) = delete;
    Shader& operator =(Shader&& other) noexcept = delete;

    void set_shader_source(const std::string& source);
    void compile();

    GLuint handle() const {return m_handle;}
    
    static std::string read_glsl_source(const std::string& path);
private:
    GLuint m_handle = 0;
};

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram& other) = delete;
    ShaderProgram(ShaderProgram&& other) noexcept = delete;
    ShaderProgram& operator =(const ShaderProgram& other) = delete;
    ShaderProgram& operator =(ShaderProgram&& other) noexcept = delete;

    void attach_shader(std::shared_ptr<Shader> shader);
    void link();
    void use() const;

    const std::vector<std::shared_ptr<Shader>>& shaders() const { return m_shaders; }

    GLuint handle() const {return m_handle;}

private:
    std::vector<std::shared_ptr<Shader>> m_shaders;

    GLuint m_handle = 0;
};

#endif
