#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"shaderClass.h"
#include"VBO.h"
#include"EBO.h"
#include"VAO.h"
#include "Texture.h"

const unsigned widthWindow = 800;
const unsigned heightWindow = 800;
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
	GLFWwindow* window = glfwCreateWindow(widthWindow, heightWindow, "Dams3D", NULL, NULL);

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
	glViewport(0, 0, widthWindow, heightWindow);


	GLfloat vertices[] =
	{	// COORDINATES		|	COLORS			|	texture coords
		-0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
		0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
		0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f,
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
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
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	//optional, to make sure we don't modified the VBOs,VAOs by accident later
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//Get reference to (i.e. get ID of ) uniform variable "scale" used in the vertex shader. A uniform variable is a kind of universal/global variable accessible from everywhere
	GLuint uniScaleID = glGetUniformLocation(shaderProgram.ID, "scale");

	// TEXTURES //
	Texture catTex = Texture("serious_cat_512_8RGBA.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	catTex.texUnit(shaderProgram, "tex0", 0);

	Texture warioTex = Texture("wario_tex_512_8RGBA.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);
	warioTex.texUnit(shaderProgram, "tex0", 1);

	//Specifying a nice deep blue color we want for the window 
	//( to replace the default white color) in the Back Buffer
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);

	//Swap the front and back buffers
	glfwSwapBuffers(window);

	float elapsedTime = glfwGetTime();

	float modelRotation = 0;
	float rotateFreq = 60;
	float prevTime = glfwGetTime();

	// Enables the Depth Buffer. To draw closer meshes above farther ones.
	glEnable(GL_DEPTH_TEST);

	//Main while loop for our application
	while (!glfwWindowShouldClose(window))
	{

		//setting the image background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//Clearing prev values in color buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//setting our application to use the shaderProgram we created before for our rendering
		shaderProgram.Activate();
		
		//Check to rotate model
		double currentTime = glfwGetTime();
		if (currentTime >= prevTime + (1/ rotateFreq)) 
		{
			modelRotation += 0.5f;
			prevTime = currentTime;
		}
		//////Transformation matrices
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		// Assigns different transformations to each matrix
		model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.f, 1.f, 0.f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		float fovRad = glm::radians(45.0f);
		float screenAspectRatio = (float)(widthWindow / heightWindow);
		float nearPlane = 0.1f;
		float farPlane = 100.0f;

		//proj = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, nearPlane, farPlane);
		proj = glm::perspective(fovRad, screenAspectRatio, nearPlane, farPlane);

		//setting matrixes uniforms variables, to be used later in the vertex shader
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		////

		elapsedTime += 0.001f;
		float curScale = std::sin(elapsedTime) * 0.5f;
		glUniform1f(uniScaleID, curScale); //Modifying uniform float var. NOTE: must be done AFTER activating the shaderProgram

		catTex.Bind();

		//telling openGL we're using the VAO vertex array as our vertices data
		VAO1.Bind();

		//Draw our geometry using TRIANGLES primitive (using the 9 first values of the EBO index buffer)
		int nbIndices = sizeof(indices) / sizeof(int);
		glDrawElements(GL_TRIANGLES, nbIndices, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//Swap the buffers between Front/Back Buffers to make sure the image is updated each frame
		glfwSwapBuffers(window);


		//Without this call, GLFW will not process the window events
		//and the window would be visible but in a not responding state
		glfwPollEvents();
	}


	//Delete all objects created before
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	catTex.Delete();
	warioTex.Delete();

	//Delete window before ending the program
	glfwDestroyWindow(window);
	//Terminate GLW before ending the program
	glfwTerminate();
	return 0;
}