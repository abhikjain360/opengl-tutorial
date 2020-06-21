#pragma once

#include <GL/glew.h>

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"

#define ASSERT(x) if(!(x)) exit(-1);
#define GLCALL(x) gl_clear_error();\
	x;\
	ASSERT(gl_call_log(#x, __FILE__, __LINE__))

void gl_clear_error();
bool gl_call_log(const char* func, const char* file, int line);

class renderer {
  public:
	void draw(const vertex_array& va, const index_buffer& ib, const shader& shader) const;
	void clear() const;
};
