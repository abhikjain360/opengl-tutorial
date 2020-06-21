#pragma once

#include <string>
#include <unordered_map>

struct shader_source {
	std::string vertex_shader;
	std::string fragment_shader;
};

class shader {
  private:
	std::string m_filepath;
	unsigned int m_rendererID;
        std::unordered_map<std::string, int> m_uniform_location_cache;

      public:
	shader(const std::string& filepath);
	~shader();

	void bind() const;
	void unbind() const;

	// set uniforms
	void set_uniform4f(const std::string& name, float v0, float v1, float v2, float v3);

  private:
	shader_source parse_shader();
	unsigned int compile_shader(unsigned int type, const std::string& source);
	unsigned int create_shader(const std::string& vertex_shader, std::string& fragment_shader);
	int get_uniform_location(const std::string& name);
};
