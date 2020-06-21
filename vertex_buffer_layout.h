#pragma once

#include <vector>
#include "renderer.h"
#include "stdlib.h"

struct vertex_buffer_element {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int get_size(unsigned int type) {
		switch(type) {
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class vertex_buffer_layout {
  private:
	std::vector<vertex_buffer_element> m_elements;
	unsigned int m_stride;
  public:
	vertex_buffer_layout()
		: m_stride(0)
	{}

	template<typename T>
	void push(int unsigned count);


	unsigned int get_stride() const {
		return m_stride;
	}
	const std::vector<vertex_buffer_element> get_elements() const {
		return m_elements;
	}
};

template<>
inline void vertex_buffer_layout::push<float>(int unsigned count) {
	m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
	m_stride += count * vertex_buffer_element::get_size(GL_FLOAT);
}

template<>
inline void vertex_buffer_layout::push<unsigned int>(int unsigned count) {
	m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
	m_stride += count * vertex_buffer_element::get_size(GL_UNSIGNED_INT);
}

template<>
inline void vertex_buffer_layout::push<unsigned char>(int unsigned count) {
	m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
	m_stride += count * vertex_buffer_element::get_size(GL_UNSIGNED_BYTE);
}
