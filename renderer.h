#pragma once

#include <GL/glew.h>

#define ASSERT(x) if(!(x)) exit(-1);

#define GLCALL(x) gl_clear_error();\
	x;\
	ASSERT(gl_call_log(#x, __FILE__, __LINE__))

void gl_clear_error();
bool gl_call_log(const char* func, const char* file, int line);
