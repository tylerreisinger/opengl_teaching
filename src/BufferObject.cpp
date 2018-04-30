#include "BufferObject.h"

#include <iostream>


BufferObject::BufferObject(BufferKind kind):
    m_kind(kind) 
{
    glGenBuffers(1, &m_handle);
    bind();
}
 
BufferObject::~BufferObject() {
    std::cout << "dtor" << std::endl;
    if(m_handle != INVALID_GL_HANDLE) {
        glDeleteBuffers(1, &m_handle);
    } 
}
 
void BufferObject::bind() {
    glBindBuffer(static_cast<GLuint>(m_kind), m_handle);
}
 
