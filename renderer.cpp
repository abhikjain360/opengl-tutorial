#include "renderer.h"

#include <iostream>

void gl_clear_error() {
	while(glGetError() != GL_NO_ERROR);
}

bool gl_call_log(const char* func, const char* file, int line) {
	if (GLenum error = glGetError()) {
		std::cout << file << ": [OpenGL error] " << error << " : " << func << " line " << line << std::endl;
		return false;
	}
	return true;
}

void renderer::draw(const vertex_array& va, const index_buffer& ib, const shader& shader) const {
	shader.bind();
	//shader.set_uniform4f("u_color", r, 0.3f, 0.8f, 1.0f);
	va.bind();
	ib.bind();

	GLCALL( glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr) );
}

void renderer::clear() const {
	GLCALL( glClear(GL_COLOR_BUFFER_BIT) );
}
