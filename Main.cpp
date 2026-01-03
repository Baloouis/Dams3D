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
#include "Camera.h"

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


	//Main Mesh ( Plane )
	GLfloat vertices[] =
	{ //     COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
		-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 2.0f,		0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		2.0f, 2.0f,		0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		2.0f, 0.0f,		0.0f, 1.0f, 0.0f
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};


	//Light Source Mesh ( Cube light )

	GLfloat lightCubeVertices[] =
	{	// COORDINATES		|	COLORS			|	texture coords
		-0.1f, -0.1f, -0.1f,
		0.1f, -0.1f, -0.1f,
		-0.1f, 0.1f, -0.1f,
		0.1f, 0.1f, -0.1f,
		-0.1f, -0.1f, 0.1f,
		0.1f, -0.1f, 0.1f,
		-0.1f, 0.1f, 0.1f,
		0.1f, 0.1f, 0.1f,

	};
	GLuint lightCubeIndices[] =
	{
		0, 1, 2,
		1, 2, 3,
		2, 3, 6,
		3, 6, 7,
		1, 3, 5,
		3, 5, 7,
		0, 1, 4,
		1, 4, 5,
		0, 2, 4,
		2, 4, 6,
		4, 5, 6,
		5, 6, 7,
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
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));


	//optional, to make sure we don't modified the VBOs,VAOs by accident later
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// TEXTURES //
	
	Texture baseTex = Texture("wooden_gate.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	baseTex.texUnit(shaderProgram, "tex0", 0);
	Texture specTex = Texture("wooden_gate_Spec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE); //GL_RED cause specular tex are only in black and white
	specTex.texUnit(shaderProgram, "tex1", 1);
	/*
	Texture warioTex = Texture("wario_tex_512_8RGBA.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);
	warioTex.texUnit(shaderProgram, "tex0", 1);

	Texture woodTex = Texture("pine_bark.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	woodTex.texUnit(shaderProgram, "tex0", 2);

	*/

	////to draw the LIGHT MESH object in the scene
	Shader lightShaderProgram("light.vert", "light.frag");

	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(lightCubeVertices, sizeof(lightCubeVertices));
	EBO lightEBO(lightCubeIndices, sizeof(lightCubeIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();
	////


	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 planePos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 planeModel = glm::mat4(1.0f);
	planeModel = glm::translate(planeModel, planePos);

	lightShaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(planeModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	//Specifying a nice deep blue color we want for the window 
	//( to replace the default white color) in the Back Buffer
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);

	//Swap the front and back buffers
	glfwSwapBuffers(window);

	float elapsedTime = glfwGetTime();

	float modelRotation = 0.1f;
	float rotateFreq = 60;
	float prevTime = glfwGetTime();

	// Enables the Depth Buffer. To draw closer meshes above farther ones.
	glEnable(GL_DEPTH_TEST);

	//creating a camera object that will be used to update transformation matrixes
	Camera camera(widthWindow, heightWindow, glm::vec3(0.f, .5f, 2.f));

	//Main while loop for our application
	while (!glfwWindowShouldClose(window))
	{
		//setting the image background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//Clearing prev values in color buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		///// CAMERA
		// Handle user input for camera mvt
		camera.Inputs(window);
		//Update settings for the camera transformation matrix
		float fovRad = 45.0f;
		float screenAspectRatio = (float)(widthWindow / heightWindow);
		float nearPlane = 0.1f;
		float farPlane = 1000.0f;
		//Update camera transformation matrix
		camera.UpdateMatrix(fovRad, nearPlane, farPlane);
		/////
		

		//// DRAW MAIN GEOMETRY
		//setting our application to use the shaderProgram we created before for our rendering
		shaderProgram.Activate();

		//Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		//Send updated camera matrix to shader program of main geometry.
		camera.ExportMatrix(shaderProgram, "camMatrix");
		baseTex.Bind();
		specTex.Bind();

		//Check to rotate model
		double currentTime = glfwGetTime();
		if (currentTime >= prevTime + (1 / rotateFreq))
		{
			//pyramidModel = glm::rotate(pyramidModel, glm::radians(modelRotation), glm::vec3(0.f, 1.f, 0.f));
			//glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));			prevTime = currentTime;
		}

		//telling openGL we're using the VAO vertex array as our vertices data
		VAO1.Bind();
		//Draw our geometry using TRIANGLES primitive (using the 9 first values of the EBO index buffer)
		int nbIndices = sizeof(indices) / sizeof(int);
		glDrawElements(GL_TRIANGLES, nbIndices, GL_UNSIGNED_INT, 0);
		////

		//// DRAW LIGHT CUBE IN THE SCENE
		lightShaderProgram.Activate();

		camera.ExportMatrix(lightShaderProgram, "camMatrix");
		lightVAO.Bind();
		nbIndices = sizeof(lightCubeIndices) / sizeof(int);
		glDrawElements(GL_TRIANGLES, nbIndices, GL_UNSIGNED_INT, 0);
		////


		//Swap the buffers between Front/Back Buffers to make sure the image is updated each frame
		glfwSwapBuffers(window);
		//Without this call, GLFW will not process the window events, and the window would be visible but in a not responding state
		glfwPollEvents();
	}


	//Delete all objects created before
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	baseTex.Delete();
	specTex.Delete();
	//warioTex.Delete();
	//woodTex.Delete();

	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShaderProgram.Delete();

	//Delete window before ending the program
	glfwDestroyWindow(window);
	//Terminate GLW before ending the program
	glfwTerminate();
	return 0;
}