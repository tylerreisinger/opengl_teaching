#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject() {
    glGenVertexArrays(1, &m_handle);
}
 
VertexArrayObject::~VertexArrayObject() {
    if(m_handle != INVALID_GL_HANDLE) {
        glDeleteVertexArrays(1, &m_handle); 
    }
}
 
void VertexArrayObject::attach_vertex_buffer(std::shared_ptr<BufferObject> buffer, GLuint attrib_idx, GLuint elem_size, GLuint elem_type, GLsizei stride, GLsizei offset, bool normalized)
{
    buffer->bind();
    glEnableVertexAttribArray(attrib_idx); 
    glVertexAttribPointer(attrib_idx, elem_size, elem_type, GL_FALSE, stride, 
        reinterpret_cast<void*>(offset));
    m_attrib_buffers.insert(std::pair(attrib_idx, std::move(buffer)));
}
 
void VertexArrayObject::bind() {
    glBindVertexArray(m_handle); 
}
 
void VertexArrayObject::unbind()
{
    glBindVertexArray(INVALID_GL_HANDLE); 
}
 
