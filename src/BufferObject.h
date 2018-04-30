#ifndef BUFFEROBJECT_H_
#define BUFFEROBJECT_H_

#include <array>
#include <vector>

#include <iostream>

#include "GlHeaders.h"

enum class BufferKind: GLuint {
    VertexBuffer = GL_ARRAY_BUFFER,
    IndexBuffer = GL_ELEMENT_ARRAY_BUFFER,
};

enum class BufferUsage: GLuint {
    StreamDraw = GL_STREAM_DRAW,
    StaticDraw = GL_STATIC_DRAW,
    DynamicDraw = GL_DYNAMIC_DRAW,
};

class BufferObject {
public:
    BufferObject(BufferKind kind);
    ~BufferObject();

    BufferObject(const BufferObject& other) = delete;
    BufferObject(BufferObject&& other) noexcept = delete;
    BufferObject& operator =(const BufferObject& other) = delete;
    BufferObject& operator =(BufferObject&& other) noexcept = delete;

    template <typename T>
    void set_data_raw(const T* data, std::size_t num_elements, BufferUsage usage) {
        std::cout << "Here I am: " <<  num_elements << std::endl;
        glBufferData(static_cast<GLuint>(m_kind), num_elements * sizeof(T), 
            data, static_cast<GLuint>(usage));
    }
    template <typename T, std::size_t N>
    void set_data(const std::array<T, N>& data, BufferUsage usage) {
        set_data_raw(data.data(), N, usage);
    }
    template <typename T>
    void set_data(const std::vector<T>& data, BufferUsage usage) {
        set_data_raw(data.data(), data.size(), usage);
    }

    GLuint handle() const {return m_handle;}

    void bind();

private:
    GLuint m_handle = INVALID_GL_HANDLE;
    BufferKind m_kind;
};

#endif
