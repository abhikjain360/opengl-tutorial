#pragma once

class vertex_buffer {
  private:
	unsigned int m_rendererID;
  public:
	vertex_buffer(const void* data, unsigned int size);
	~vertex_buffer();

	void bind() const;
	void unbind() const;
};
