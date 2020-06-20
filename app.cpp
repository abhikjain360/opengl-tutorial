#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
	unsigned int id = glCreateShader(type);
	// use getString if source actually doesn't exist as just a pointer
	// memory may not exist even if pointer points
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		char* message = (char*)alloca(lenght * sizeof(char));
		glGetShaderInfoLog(id, lenght, &lenght, message);

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int create_shader(const std::string& vertex_shader, std::string& fragment_shader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
	unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

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

	std::cout << glGetString(GL_VERSION) << std::endl;

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	float positions[6] = {
		-0.5f, -0.5f,
                 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	shader_source source = parse_shader("shaders/basic.shader");

	unsigned int shader = create_shader(source.vertex_shader, source.fragment_shader);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}
