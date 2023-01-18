#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

unsigned int createShader(unsigned int shaderType, const char** shaderSource);
unsigned int createShaderProgram(unsigned int shaders[], int count = 0);
void deleteShaders(unsigned int shaders[], int count = 0);

void bindVertexData(unsigned int VBO, unsigned int VAO, const float vertices[], unsigned int size);

// settings
constexpr unsigned int screenWidth = 800;
constexpr unsigned int screenHeight = 600;
const char* screenTitle = "OpenGL";

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, screenTitle, nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile shader program
	// --------------------------------
	const unsigned int vertexShader = createShader(GL_VERTEX_SHADER, &vertexShaderSource);

	const unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, &fragmentShaderSource);

	// link shaders
	unsigned int shaders[] = { vertexShader, fragmentShader };
	const unsigned int shaderProgram = createShaderProgram(shaders, 2);

	deleteShaders(shaders);

	// vertex data and buffers
	// -----------------------
	constexpr float firstTriangleVertices[] = {
		// first triangle
		-0.9f, -0.8f, 0.0f, // left
		 0.1f, -0.1f, 0.0f, // right
		-0.4f,  0.4f, 0.0f // top
	};

	constexpr float secondTriangleVertices[] = {
		0.0f, 0.4f, 0.0f, // left
		0.8f, 0.8f, 0.0f, // right
		0.4f, -0.6f, 0.0f // top
	};

	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	bindVertexData(VBOs[0], VAOs[0], firstTriangleVertices, sizeof(firstTriangleVertices));
	bindVertexData(VBOs[1], VAOs[1], secondTriangleVertices, sizeof(secondTriangleVertices));

	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(shaderProgram);
		// draw first triangle
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// draw second triangle
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(1, VBOs);
	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

unsigned createShader(unsigned int shaderType, const char** shaderSource)
{
	const unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, shaderSource, nullptr);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}

unsigned createShaderProgram(unsigned int shaders[], int count)
{
	const unsigned int shaderProgram = glCreateProgram();

	if (shaders != nullptr && count > 0)
	{
		for (int i = 0; i < count; ++i)
		{
			glAttachShader(shaderProgram, shaders[i]);
		}
		glLinkProgram(shaderProgram);

		int success;

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}

	return shaderProgram;
}

void deleteShaders(unsigned int shaders[], int count)
{
	if (shaders != nullptr && count > 0)
	{
		for (int i = 0; i < count; ++i)
		{
			glDeleteShader(shaders[i]);
		}
	}
}

void bindVertexData(unsigned int VBO, unsigned int VAO, const float vertices[], unsigned size)
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}