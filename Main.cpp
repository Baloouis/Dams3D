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
#include "Mesh.h"

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
	Vertex vertices[] =
	{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
		Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	//Light Source Mesh ( Cube light )

	Vertex lightCubeVertices[] =
	{ //     COORDINATES     //
		Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
	};
	GLuint lightCubeIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};

	//// TEXTURES //
	Texture diffuseTex = Texture("wooden_gate.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture specTex = Texture("wooden_gate_Spec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE); //GL_RED cause specular tex are only in black and white

	Texture textures[] = {
		diffuseTex,
		specTex,
	};
	////


	Shader shaderProgram("default.vert", "default.frag");

	//Creating std::vectors from arrays using the range constructor with 1rst argument being the start address of the array and 2nd argument being the last adress of the array
	std::vector<Vertex> vecVertices(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector<GLuint> vecIndices(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<Texture> vecTextures(textures, textures + sizeof(textures) / sizeof(Texture));

	Mesh floorMesh = Mesh(vecVertices, vecIndices, vecTextures);


	////to draw the LIGHT MESH object in the scene
	// Shader for light cube
	Shader lightShaderProgram("light.vert", "light.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> vecLightVerts(lightCubeVertices, lightCubeVertices + sizeof(lightCubeVertices) / sizeof(Vertex));
	std::vector <GLuint> vecLightInd(lightCubeIndices, lightCubeIndices + sizeof(lightCubeIndices) / sizeof(GLuint));
	// Create light mesh ( using same textures as the other mesh as placeholders )
	Mesh lightMesh(vecLightVerts, vecLightInd, vecTextures);
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
		floorMesh.Draw(shaderProgram, camera);
		////

		//// DRAW LIGHT CUBE IN THE SCENE
		lightMesh.Draw(lightShaderProgram, camera);
		////

		//Swap the buffers between Front/Back Buffers to make sure the image is updated each frame
		glfwSwapBuffers(window);
		//Without this call, GLFW will not process the window events, and the window would be visible but in a not responding state
		glfwPollEvents();
	}


	//Delete all objects created before


	diffuseTex.Delete();
	specTex.Delete();

	shaderProgram.Delete();
	lightShaderProgram.Delete();

	//Delete window before ending the program
	glfwDestroyWindow(window);
	//Terminate GLW before ending the program
	glfwTerminate();
	return 0;
}