#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <stb/stb_image.h>

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
	{	// COORDINATES		|	COLORS			|	texture coords
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	//lower left
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,	//upper left
		0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,	//upper right
		0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,	//lower right
	};

	GLuint indices[] =
	{
		0, 1, 2, //upper-left triangle
		2, 3, 0, //Lower-right triangle
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
	stbi_set_flip_vertically_on_load(true); //to correct the fact that openGl and stb don't read an image in the same direction
	//Getting the image and storing it in a unsigned char array called "imgBytes"
	int widthImg, heightImg, numColCh;
	unsigned char* imgData = stbi_load("wario_tex_512.png", &widthImg, &heightImg, &numColCh, 0);
	if(!imgData)
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	//Use the texture unit 0 for our texture. Texture Units are like slots for textures. 
	//They generally come in bundle of 16 units and can be used so the fragment shader works on all those textures at the same time.
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, texture);

	//Modifying the binded texture's settings. Here we say we want to use GL_NEAREST as the protocol to use when scaling the image
	//Could also be GL_LINEAR ( check online on https://learnopengl.com/Getting-started/Textures to see difference )
	//MIN and MAG corresponds to the Magnifying and Minifying operations ie scaling up or down. The texture filtering setting needs to be done for both of them separately.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Setting the protocol to use for mapping (ie when we go farther than the pixels of the texture, thenthe texture is repeated/clamped/etc...)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	if (imgData)
	{
		//Generate the Image with for the currently bounded texture with given settings ( NB : GL_UNSIGNED_BYTE is the data type of our pixels )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
		//Generate the mipmaps for the texture (smaller resolution versions of the texture used when the img is far away)
		glGenerateMipmap(GL_TEXTURE_2D);
	}


	stbi_image_free(imgData); //delete the data for the img
	glBindTexture(GL_TEXTURE_2D, 0); //unbind the texture now, so we dont accidentally make changes to it later on.

	//Set reference in the fragment shader to our texture
	GLuint uniTex0ID = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(uniTex0ID, 0); //Modifying uniform int var corresponding to the texture unit ID. NOTE: must be done AFTER activating the shaderProgram






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
		
		glBindTexture(GL_TEXTURE_2D, texture);

		//telling openGL we're using the VAO vertex array as our vertices data
		VAO1.Bind();

		//Draw our geometry using TRIANGLES primitive (using the 9 first values of the EBO index buffer)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

	glDeleteTextures(1, &texture);

	//Delete window before ending the program
	glfwDestroyWindow(window);
	//Terminate GLW before ending the program
	glfwTerminate();
	return 0;
}