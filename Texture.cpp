#include"Texture.h"
#include <stb/stb_image.h>


Texture::Texture(const char* imageName, const char* textureType, GLuint slot, GLenum format, GLenum pixelType)
{
	//assign the type of the texture to the Texture object
	texType = textureType;

	stbi_set_flip_vertically_on_load(true); //flip the image to correct the fact that openGl and stb don't read an image in the same direction

	//Getting the image and storing it in a unsigned char array called "imgBytes"
	int widthImg, heightImg, numColCh;
	//reads the image from a file and stores it in bytes
	unsigned char* imgData = stbi_load(imageName, &widthImg, &heightImg, &numColCh, 0);
	if (!imgData)
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	//Generates a new openGL Texture object
	glGenTextures(1, &ID);
	//Assign our texture to a Texture Unity. Texture Units are like slots for textures. 
	//They generally come in bundle of 16 units and can be used so the fragment shader works on all those textures at the same time.
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	//Modifying the binded texture's settings. Here we say we want to use GL_NEAREST as the protocol to use when scaling the image
	//Could also be GL_LINEAR ( check online on https://learnopengl.com/Getting-started/Textures to see difference )
	//MIN and MAG corresponds to the Magnifying and Minifying operations ie scaling up or down. The texture filtering setting needs to be done for both of them separately.
	 (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Setting the protocol to use for mapping (ie when we go farther than the pixels of the texture, then the texture is repeated/clamped/etc...)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	//Generate the Image with for the currently bounded texture with given settings ( NB : GL_UNSIGNED_BYTE is the data type of our pixels )
	//IMPORTANT : we may have an error at the glGenerateMipmap() line if the internal format of the texture does not correspond to the internal format specified here (ie GL_RGBA)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, imgData);
	//Generate the mipmaps for the texture (smaller resolution versions of the texture used when the img is far away)
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imgData); //delete the data for the img	as it is already in the OpenGL Texture object
	glBindTexture(GL_TEXTURE_2D, 0); //unbind the texture now, so we dont accidentally make changes to it later on.
}

//Exports a reference to our texture in the fragment shader 
void Texture::AssignTexUnit(Shader& shader, const char* uniformName, GLuint unit)
{
	// Gets the location of the uniform
	GLuint uniTexID = glGetUniformLocation(shader.ID, uniformName);
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Sets the value of the uniform
	glUniform1i(uniTexID, unit); //Modifying uniform int var corresponding to the texture unit ID. NOTE: must be done AFTER activating the shaderProgram
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}
