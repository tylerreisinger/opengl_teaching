#ifndef VERTEXARRAYOBJECT_H_
#define VERTEXARRAYOBJECT_H_

#include "BufferObject.h"
#include "GlHeaders.h"

#include <memory>
#include <unordered_map>

class VertexArrayObject {
public:
    VertexArrayObject();
    ~VertexArrayObject();

    VertexArrayObject(const VertexArrayObject& other) = delete;
    VertexArrayObject(VertexArrayObject&& other) noexcept = delete;
    VertexArrayObject& operator =(const VertexArrayObject& other) = delete;
    VertexArrayObject& operator =(VertexArrayObject&& other) noexcept = delete;

    void attach_vertex_buffer(std::shared_ptr<BufferObject> buffer, GLuint attrib_idx,
        GLuint elem_size, GLuint elem_type, GLsizei stride, GLsizei offset, bool normalized=false);

    void bind();

    static void unbind();

    GLuint handle() const { 
        return m_handle; 
    }

private:
    std::unordered_map<GLuint, std::shared_ptr<BufferObject>> m_attrib_buffers;
    GLuint m_handle = INVALID_GL_HANDLE;    

};

#endif
