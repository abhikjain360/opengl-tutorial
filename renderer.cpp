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
