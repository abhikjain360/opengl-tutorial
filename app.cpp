#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "renderer.h"

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "shader.h"

int main(void) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	{
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

		vertex_array va;
		vertex_buffer vb(positions, 4 * 2 * sizeof(float));

		vertex_buffer_layout layout;
		layout.push<float>(2);
		va.addbuffer(vb, layout);

		index_buffer ib(indices, 6);

		shader shader("shaders/basic.shader");
		shader.bind();
		shader.set_uniform4f("u_color", 0.8f, 0.3f, 0.8f, 1.0f);

		va.unbind();
		shader.unbind();
		vb.unbind();
		ib.unbind();

		renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while ( !glfwWindowShouldClose(window) ) {
			/* Render here */
			renderer.clear();

			shader.bind();
			shader.set_uniform4f("u_color", r, 0.3f, 0.8f, 1.0f);

			renderer.draw(va, ib, shader);

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
	}
	glfwTerminate();
	return 0;
}
