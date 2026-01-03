#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include "shaderClass.h"

class Texture
{
public:
	GLuint ID;
	GLenum texType;
	GLuint unit;

	Texture(const char* imageData, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);

	// Binds a texture
	void Bind();
	// Uninds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};
#endif