#include "Shader.h"

#include <fstream>
#include <iostream>

std::string Shader::read_glsl_source(const std::string& path) {
    std::ifstream stream;
    stream.open(path);
    if(stream.is_open()){
        stream.seekg(0, std::ios::end);
        auto size = stream.tellg();
        stream.seekg(0, std::ios::beg);

        std::cout << size << std::endl;
        std::string result(size, '\0');
        stream.read(result.data(), size);
        return result;
    } else {
        throw std::runtime_error("File '" + path + "' was not found.");
    }
}
 
Shader::Shader(ShaderKind kind) {
    m_handle = glCreateShader(static_cast<GLuint>(kind));
}
 
Shader::~Shader() {
    if(m_handle != INVALID_GL_HANDLE) {
        glDeleteShader(m_handle);
    }
}
 
void Shader::set_shader_source(const std::string& source) { 
    const char* string_array = source.data();
    const int len = source.size();
    glShaderSource(m_handle, 1, &string_array, &len);
}
 
void Shader::compile() {
    glCompileShader(m_handle);
    int status;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);
    if(!status) {
        int log_size;
        glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &log_size);
        int len_out = 0;
        std::string info_log(log_size+1, '\0');
        glGetShaderInfoLog(m_handle, log_size, &len_out, info_log.data());
        throw std::runtime_error("Shader compilation failed!\nCompilation Log:" + info_log);
    }
}
 
ShaderProgram::ShaderProgram() {
    m_handle = glCreateProgram();
}
 
ShaderProgram::~ShaderProgram() {
    if(m_handle != INVALID_GL_HANDLE) {
        glDeleteProgram(m_handle);
    }
}
 
void ShaderProgram::attach_shader(std::shared_ptr<Shader> shader) {
    glAttachShader(m_handle, shader->handle());
    m_shaders.push_back(std::move(shader));
}
 
void ShaderProgram::link() {
    glLinkProgram(m_handle);
    int status = 0;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
    if(!status) {
        int log_size;
        glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &log_size);
        int len_out = 0;
        std::string info_log(len_out+1, '\0');
        glGetProgramInfoLog(m_handle, log_size, &len_out, info_log.data());
        std::cout << info_log << std::endl;
        throw std::runtime_error("Shader program linking failed!\nLog:" + info_log);
    }
}
 
void ShaderProgram::use() const {
    glUseProgram(m_handle);
}
 
