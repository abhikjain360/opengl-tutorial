#pragma once

#include "vertex_buffer.h"

class vertex_buffer_layout;

class vertex_array {
  private:
	unsigned int m_rendererID;
  public:
	vertex_array();

	~vertex_array();

	void addbuffer(const vertex_buffer& vb, const vertex_buffer_layout& layout);

	void bind() const;
	void unbind() const;
};
