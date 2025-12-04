#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main()
{
	glfwInit();

	//Tell GLFW what version of OpenGL we're using (here OpenGL 3.4)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	//Tell GLFW we're using the CORE profile
	//ie we only want to use/have the modern functions of OpenGL in the project
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	//Create 800x800 window named "DamsModelLoader"
	GLFWwindow* window = glfwCreateWindow(800, 800, "DamsModelLoader", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//this tells GLFW to show the window we just created
	//by introducing the window to the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	//Specify the viewport of OpenGL in the window, 
	// here the viewport goes from x=0, y=0, to x=800, y=800
	glViewport(0, 0, 800, 800);


	//Get reference for vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//assign our source code (ie vertexShaderSource) to our vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//Compile the vertex shader to be able to use it after
	glCompileShader(vertexShader);

	//Same steps as above but for fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	//OPTIONAL : Checking if the fragment shader compilation was a success
	int  success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//Attach our shaders to a shader program, to wrap them together
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader); // attaching vertexShader to our program
	glAttachShader(shaderProgram, fragmentShader); // attaching fragment to our program
	glLinkProgram(shaderProgram); //link our shader program ie link both our vertexShader & fragmentShader together

	//we can delete the shaders ref from before at they are in the shader program itself now
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//To send info from the GPU to the CPU we do it in big batches
	//because it takes a relatively long time.
	//To do this we store info batches in buffers that we then send to the CPU

	//Creating the reference container for the Vertex Buffer Object, to store data on the vertices
	GLuint VBO;

	//Creating  the reference container for the Vertex Array Buffers that store pointers to multiples VBOs,
	//tells OpenGL how to interpret the VBOs,
	//and is useful to quickly be able to switch between different VBOs
	//NB: the VAO NEEDS to be generated BEFORE the VBO
	GLuint VAO;

	//Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Make the VAO the current Vertex Array Object by bindint it
	glBindVertexArray(VAO);

	//Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Link our vertices data into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Configure the Vertex Attribute data so that OpenGL knows how to read the VBO
	//ie here we specify that each vertex in our VBO contains only 3 float values ( x, y and z coordinates) 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//Enable the vertex Attribute so that OpenGL knows how to use it
	glEnableVertexAttribArray(0);

	//Bind both the VBO and VAO to 0 so that we don't modify the VAO or VBO by accident
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinding the VBO by binding it to 0
	glBindVertexArray(0); //unbinding the VAO by binding it to 0


	//Specifying a nice deep blue color we want for the window 
	//( to replace the default white color) in the Back Buffer
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);

	//Swap the front and back buffers
	glfwSwapBuffers(window);


	//Main while loop for our application
	while (!glfwWindowShouldClose(window))
	{

		//setting the image background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//setting our application to use the shaderProgram we created before for our rendering
		glUseProgram(shaderProgram);
		//telling openGL we're using the VAO vertex array as our vertices data
		glBindVertexArray(VAO);

		//Draw our geometry using TRIANGLES primitive, going from index = 0 to 3 in our data ( we only have 3 vertices in our dataset rn)
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//Swap the buffers between Front/Back Buffers to make sure the image is updated each frame
		glfwSwapBuffers(window);


		//Without this call, GLFW will not process the window events
		//and the window would be visible but in a not responding state
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}