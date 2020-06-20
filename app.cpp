#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if(!(x)) exit(-1);

#define GLCALL(x) gl_clear_error();\
	x;\
	ASSERT(gl_call_log(#x, __FILE__, __LINE__))

static void gl_clear_error() {
	while(glGetError() != GL_NO_ERROR);
}

static bool gl_call_log(const char* func, const char* file, int line) {
	if (GLenum error = glGetError()) {
		std::cout << file << ": [OpenGL error] " << error << " : " << func << " line " << line << std::endl;
		return false;
	}
	return true;
}

struct shader_source {
	std::string vertex_shader;
	std::string fragment_shader;
};

static shader_source parse_shader(const std::string& filepath) {
	std::ifstream file(filepath);

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
		}
		else {
			ss[(int)type] << line << "\n";
		}

	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int compile_shader(unsigned int type, const std::string& source) {
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

static unsigned int create_shader(const std::string& vertex_shader, std::string& fragment_shader) {
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

int main(void) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(5);

	GLCALL( std::cout << glGetString(GL_VERSION) << std::endl );

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	float positions[] = {
		-0.5f, -0.5f,
                 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};

	unsigned int indices[] = {
		0, 1, 2, // triangle 1
		2, 3, 0  // triangle 2
	};

	unsigned int buffer;
	GLCALL( glGenBuffers(1, &buffer) );
	GLCALL( glBindBuffer(GL_ARRAY_BUFFER, buffer) );
	GLCALL( glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW) );

	GLCALL( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0) );
	GLCALL( glEnableVertexAttribArray(0) );

	unsigned int ibo;
	GLCALL( glGenBuffers(1, &ibo) );
	GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo) );
	GLCALL( glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW) );

	shader_source source = parse_shader("shaders/basic.shader");
	unsigned int shader = create_shader(source.vertex_shader, source.fragment_shader);
	GLCALL( glUseProgram(shader) );

	int location = glGetUniformLocation(shader, "u_color");
	GLCALL( ASSERT(location != -1) );
	GLCALL( glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f) );

	float r = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */
	while ( !glfwWindowShouldClose(window) ) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		GLCALL( glUniform4f(location, r, 0.3f, 0.8f, 1.0f) );
		GLCALL( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr) );

		if (r > 1.0f)
			increment = -0.05f;
		else if (r < 0.0f)
			increment =  0.05f;

		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GLCALL( glDeleteProgram(shader) );

	glfwTerminate();
	return 0;
}
