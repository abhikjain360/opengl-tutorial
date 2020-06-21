#include "vertex_array.h"
#include "vertex_buffer_layout.h"
#include "renderer.h"

#include <stdlib.h>

vertex_array::vertex_array() {
	GLCALL( glGenVertexArrays(1, &m_rendererID) );
}

vertex_array::~vertex_array() {
	GLCALL( glDeleteVertexArrays(1, &m_rendererID) );
}

void vertex_array::bind() const {
	GLCALL( glBindVertexArray(m_rendererID) );
}

void vertex_array::unbind() const {
	GLCALL( glBindVertexArray(0) );
}

void vertex_array::addbuffer(const vertex_buffer& vb, const vertex_buffer_layout& layout) {
	bind();
	vb.bind();
	const auto& elements = layout.get_elements();
	unsigned int offset;

	for(unsigned int i = 0; i < elements.size(); ++i) {
		const auto& element = elements[i];
		GLCALL( glEnableVertexAttribArray(i) );
		GLCALL( glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.get_stride(), 0) );
		offset += element.count * vertex_buffer_element::get_size(element.type);
	}

}
