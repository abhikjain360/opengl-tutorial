#pragma once

class index_buffer {
  private:
	unsigned int m_rendererID;
	unsigned int m_count;

  public:
	index_buffer(const unsigned int* data, unsigned int countn);
	~index_buffer();

	void bind() const;
	void unbind() const;

	unsigned int get_count() const { return m_count; }
};
