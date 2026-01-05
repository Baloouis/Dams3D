#include"EBO.h"

// Constructor that generates a Element Buffer Object and links it to indices. An EBO is an index buffer, used to store the indices corresponding to the all the vertices of a model ( to help reduce memory usage overall )
EBO::EBO(std::vector<GLuint> indices)
{
	//Generate the VBO with only 1 object
	glGenBuffers(1, &ID);
	//Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	//Link our vertices data into the VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

// Binds the EBO
void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}
// Unbinds the EBO
void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //unbinding the EBO by binding it to 0
}
// Deletes the EBO
void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}