#include <stdlib.h>

#include "vertex_buffer.h"

#include "renderer.h"

vertex_buffer::vertex_buffer(const void* data, unsigned int size) {
	GLCALL( glGenBuffers(1, &m_rendererID) );
	GLCALL( glBindBuffer(GL_ARRAY_BUFFER, m_rendererID) );
	GLCALL( glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW) );
}

vertex_buffer::~vertex_buffer() {
	GLCALL( glDeleteBuffers(1, &m_rendererID) );
}

void vertex_buffer::bind() const {
	GLCALL( glBindBuffer(GL_ARRAY_BUFFER, m_rendererID) );
}

void vertex_buffer::unbind() const {
	GLCALL( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}
