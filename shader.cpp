#include "shader.h"
#include "renderer.h"

#include <iostream>
#include <sstream>
#include <fstream>



shader::shader(const std::string& filepath)
	: m_filepath(filepath), m_rendererID(0)
{
	shader_source source = parse_shader();
	m_rendererID = create_shader(source.vertex_shader, source.fragment_shader);
}

shader::~shader() {
	GLCALL( glDeleteProgram(m_rendererID) );
}



shader_source shader::parse_shader() {
	std::ifstream file(m_filepath);

	enum class shader_type {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::stringstream ss[2];
	std::string line;
	shader_type type = shader_type::NONE;
	while(getline(file, line)) {
		if (line.find("// shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = shader_type::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = shader_type::FRAGMENT;
		} else {
			ss[(int)type] << line << "\n";
		}
	}
	return { ss[0].str(), ss[1].str() };
}

unsigned int shader::compile_shader(unsigned int type, const std::string& source) {
	GLCALL( unsigned int id = glCreateShader(type) );
	// use getString if source actually doesn't exist as just a pointer
	// memory may not exist even if pointer points
	const char* src = source.c_str();
	GLCALL( glShaderSource(id, 1, &src, nullptr) );
	GLCALL( (glCompileShader(id)); )

	int result;
	GLCALL( glGetShaderiv(id, GL_COMPILE_STATUS, &result) );
	if (result == GL_FALSE) {
		int lenght;
		GLCALL( glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght) );
		char* message = (char*)alloca(lenght * sizeof(char));
		GLCALL( glGetShaderInfoLog(id, lenght, &lenght, message) );

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;

		GLCALL( glDeleteShader(id) );
		return 0;
	}

	return id;
}

unsigned int shader::create_shader(const std::string& vertex_shader, std::string& fragment_shader) {
	GLCALL( unsigned int program = glCreateProgram() );
	unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
	unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	GLCALL( glAttachShader(program, vs) );
	GLCALL( glAttachShader(program, fs) );

	GLCALL( glLinkProgram(program) );
	GLCALL( glValidateProgram(program) );

	GLCALL( glDeleteShader(vs) );
	GLCALL( glDeleteShader(fs) );

	return program;
}

void shader::bind() const {
	GLCALL( glUseProgram(m_rendererID) );
}

void shader::unbind() const {
	GLCALL( glUseProgram(0) );
}

int shader::get_uniform_location(const std::string& name) {
	if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
		return m_uniform_location_cache[name];

	int location = glGetUniformLocation(m_rendererID, name.c_str());
	if (location == -1)
		std::cout << "Warning: uniform " << name << "doesn't exist" << std::endl;
	m_uniform_location_cache[name] = location;
	return location;
}

void shader::set_uniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCALL( glUniform4f(get_uniform_location(name), v0, v1, v2, v3) );
}
