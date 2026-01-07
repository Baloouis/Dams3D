#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include "shaderClass.h"

class Texture
{
public:
	GLuint ID;
	const char* texType;
	GLuint unit;

	Texture(const char* imageData, const char* texType, GLuint slot);

	// Assigns a texture unit to a texture
	void AssignTexUnit(Shader& shader, const char* uniform, GLuint unit);

	// Binds a texture
	void Bind();
	// Uninds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};
#endif