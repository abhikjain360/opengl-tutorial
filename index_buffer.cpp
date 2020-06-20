#include <stdlib.h>

#include "index_buffer.h"

#include "renderer.h"

index_buffer::index_buffer(const unsigned int* data, unsigned int countn)
	: m_count(countn)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCALL( glGenBuffers(1, &m_rendererID) );
	GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID) );
	GLCALL( glBufferData(GL_ELEMENT_ARRAY_BUFFER, countn * sizeof(unsigned int), data, GL_STATIC_DRAW) );
}

index_buffer::~index_buffer() {
	GLCALL( glDeleteBuffers(1, &m_rendererID) );
}

void index_buffer::bind() const {
	GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID) );
}

void index_buffer::unbind() const {
	GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
}
