#include "libs.h"

Vertex vertices[] = 
{
	/* Position */						/* Color */			  /* Coordinate */
	glm::vec3(-0.5f, 0.5f, 0.f),  glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f),
	glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f),
	glm::vec3(0.5f, -0.5f, 0.f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),
	glm::vec3(0.5f,  0.5f, 0.f),  glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 1.f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indicies[] =
{
	0,1,2,	//Triangle 1
	0,2,3	//Triangle 2
};
unsigned nrOfIndicies = sizeof(indicies) / sizeof(GLuint);

void updateInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void framebuffer_rezise_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}

bool loadShaders(GLuint &program)
{	
	bool loadSuccess = true;
	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	/* Create Vertex Shader */
	in_file.open("vertex_core.glsl");
	
	if (in_file.is_open())
	{
		/* check if the vertex_core file is opened */
		while (std::getline(in_file, temp))
		{
			src += temp + "\n";
		}
	}
	else
	{
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE" << "\n";
		loadSuccess = false;
	}
	in_file.close();

	GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertSrc = src.c_str();
	glShaderSource(vertexshader, 1, &vertSrc, NULL);
	glCompileShader(vertexshader);

	/* Check Shader Compile Status */
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
		 std::cout <<"ERROR::LOADSHADERS::COULD_NOT_COMPILE_SHADERS" << "\n";
		 std::cout << infoLog << "\n";
		 loadSuccess = false;
	}

	temp = "";
	src = "";

	/* Create Fragment Shader*/
	in_file.open("fragment_core.glsl");
	
	/* Check if the fragment_core file is opened */
	if (in_file.is_open())
	{
		while (std::getline(in_file, temp))
		{
			src += temp + "\n"; 
		}
	}
	else
	{
		std::cout << "ERROR::LOADFRAGMENTS::COULD_NOT_OPEN_FRAGMENT_FILE" << "\n";
		loadSuccess = false;
	}
	in_file.close();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragSrc = src.c_str();
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	/* Check Fragment Compile Status */
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);	
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR_LOADSHADERS::COULD_NOT_COMPILE_FRAGMENT_SHADER" << "\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}
	
	/* Create Programs To Attach Shader*/
	program = glCreateProgram();

	glAttachShader(program, vertexshader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR_LOADFRAGMENTS::COULD_NOT_LINK_PROGRAM" << "\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}

	/*End Function */
	glUseProgram(0);
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentShader);

	return loadSuccess;
}

int main(void)
{
	/* Initialize GLFW */
	if (!glfwInit())
		return -1;

	/* Setup Window Variables*/
	const int WINDOW_WIDTH = 400;
	const int WINDOW_HEIGHT = 543;
	const char* WINDOW_TITLE = "OpenGL ES Demo";
	const int framebufferWidht = 0;
	const int framebufferHeight = 0;	

	
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwSetFramebufferSizeCallback(window, framebuffer_rezise_callback);
	glfwMakeContextCurrent(window);

	/* Initialize GLEW */
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR:MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	/* OpenGL Settings */
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	/* Shader Initialize */
	GLuint core_program;
	if(!loadShaders(core_program))
	{
		glfwTerminate();
	}

	/* Generate VAO and Bind It */
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	/*Generate VBO and Bind and Send Data*/
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*Generate EBO and Bind and Send Data*/
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	/* Set VERTEXATRIBPOINTERS AND ENABLE  (INPUT ASSEMBLY) */
	/* Position*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	/* Color */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	/* TexCoord */
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	/* Bind VAO 0 */
	glBindVertexArray(0);

	/* Texture Character Initialize */
	int imageWidth = 0;
	int imageHeight = 0;
	/*Get the character image from Images Folder inside the project file */
	unsigned char* image = SOIL_load_image("Images/mario.png", &imageWidth, &imageHeight, NULL, SOIL_LOAD_RGBA);
	
	GLuint texture0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::LOADING_TEXTURE_FAILED" << "\n";
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	/* Setup Transform */
	glm::mat4 ModelMatrix(1.f);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f));

	glUseProgram(core_program);
	glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

	glUseProgram(0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Poll for and process events */
		glfwPollEvents();

		/* Update Input */
		updateInput(window);

		/* Draw --- Clear */
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		/* Use Program */
		glUseProgram(core_program);

		/* Update Uniform */
		glUniform1i(glGetUniformLocation(core_program, "texture0"), 0);

		/* Update Rotatition */
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.2f), glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f));
		
		glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

		/* Activate Texture */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);

		/* Bind Vertex Array Object */
		glBindVertexArray(VAO);

		/* Draw */
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		glDrawElements(GL_TRIANGLES, nrOfIndicies, GL_UNSIGNED_INT, 0);
		
		/* End Draw */
		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D,0);
	}

	/*End Program*/
	glfwDestroyWindow(window);
	glfwTerminate();

	/*Delete Program */
	glDeleteProgram(core_program);

	return 0;
}