#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shaderClass.h"
#include"VBO.h"
#include"EBO.h"
#include"VAO.h"

int main()
{
	glfwInit();

	//Tell GLFW what version of OpenGL we're using (here OpenGL 3.4)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	//Tell GLFW we're using the CORE profile
	//ie we only want to use/have the modern functions of OpenGL in the project
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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


	GLfloat vertices[] =
	{	// COORDINATES							|	COLORS		//
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,	0.8f, 0.3f, 0.02f,	//l-l (ie lower-left)
		 0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,	0.8f, 0.3f, 0.02f,	//l-r
		 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,	1.0f, 0.6f, 0.32f, //upper-corner
		 -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,	0.9f, 0.45f, 0.17f, //inner-l
		 0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,		0.9f, 0.45f, 0.17f, //i-r
		 0.f, -0.5f * float(sqrt(3)) / 3, 0.0f,		0.8f, 0.3f, 0.02f, //i-down
	};

	GLuint indices[] =
	{
		0, 3, 5, //Lower-left triangle
		3, 2, 4, //Lower right triangle
		5, 4, 1 //Upper triangle
	};

	Shader shaderProgram("default.vert", "default.frag");

	//To send info from the GPU to the CPU we do it in big batches
	//because it takes a relatively long time.
	//To do this we store info batches in buffers that we then send to the CPU

	VAO VAO1;
	VAO1.Bind(); 	//Make the VAO the current Vertex Array Object by bindint it

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	//optional, to make sure we don't modified the VBOs,VAOs by accident later
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//Get reference to (i.e. get ID of ) uniform variable "scale" used in the vertex shader. A uniform variable is a kind of universal/global variable accessible from everywhere
	GLuint uniScaleID = glGetUniformLocation(shaderProgram.ID, "scale");

	//Specifying a nice deep blue color we want for the window 
	//( to replace the default white color) in the Back Buffer
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);

	//Swap the front and back buffers
	glfwSwapBuffers(window);

	float elapsedTime = glfwGetTime();

	//Main while loop for our application
	while (!glfwWindowShouldClose(window))
	{

		//setting the image background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//setting our application to use the shaderProgram we created before for our rendering
		shaderProgram.Activate();

		elapsedTime += 0.001f;
		float curScale = std::sin(elapsedTime) * 0.5f;
		glUniform1f(uniScaleID, curScale); //Modifying uniform float var. NOTE: must be done AFTER activating the shaderProgram
		//telling openGL we're using the VAO vertex array as our vertices data
		VAO1.Bind();

		//Draw our geometry using TRIANGLES primitive (using the 9 first values of the EBO index buffer)
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//Swap the buffers between Front/Back Buffers to make sure the image is updated each frame
		glfwSwapBuffers(window);


		//Without this call, GLFW will not process the window events
		//and the window would be visible but in a not responding state
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	VAO1.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}