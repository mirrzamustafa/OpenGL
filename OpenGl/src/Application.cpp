#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char * str = &source[0];

	glShaderSource(id, 1, &str, nullptr);
	glCompileShader(id);

	// only Error Handling below 
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed To Compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " Shader !" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}
	// till here
	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);


	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main()
{
	GLFWwindow * window;
	glfwInit();
	if (glfwInit())
	{
		std::cout << "GLFW WORKS" << std::endl;
	}
	if (!glfwInit())
	{
		return -1;
	}

	window = glfwCreateWindow(640, 380, "Open GL Window", NULL, NULL);

	if (!window)
	{
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewInit();

	if (glewInit() == GLEW_OK)
	{
		std::cout << "GLEW WORKS" << std::endl;
		std::cout << glGetString(GL_VERSION) << std::endl;
	}

	float positions[12] = {

		-0.5f,0.5f,  //0
		-0.5f,-0.5f, //1
		 0.5f,-0.5f, //2
		 0.5f,0.5f   //3  
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	/* I changed it 6 to 4 */
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	unsigned int indices[] = {
		0 , 1 , 2,
		3 , 2 , 0
	};

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	std::string vertexShader1 =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;"
		"\n"
		"void main()\n"
		"{\n"
		" gl_Position = position;\n"
		"}\n";
	std::string fragmentShader1 =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;"
		"\n"
		"void main()\n"
		"{\n"
		" color = vec4( 1.0, 0.0, 0.0, 1.0);\n"
		"}\n";

	unsigned int shader = CreateShader(vertexShader1, fragmentShader1);
	glUseProgram(shader);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	if (glfwWindowShouldClose(window))
	{
		glDeleteProgram(shader);
		glfwTerminate();
	}
	return 0;
}
