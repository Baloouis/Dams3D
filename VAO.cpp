#include"VAO.h"


//The Vertex Array Object is a references container that store pointers to multiples VBOs,
//it also tells OpenGL how to interpret the VBOs,
//and is useful to quickly be able to switch between different VBOs
//NB: the VAO NEEDS to be generated BEFORE the VBOs
VAO::VAO()
{
	//Generate the VAO only 1 object
	glGenVertexArrays(1, &ID);
}

//Links a VBO Attribute such as a position or color to the VAO
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	//Configure the Vertex Attribute data so that OpenGL knows how to read the VBO
	//ie here we specify that each vertex in our VBO contains only 3 float values ( x, y and z coordinates) 
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	//Enable the vertex Attribute so that OpenGL knows how to use it
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}
// Binds the VAO
void VAO::Bind()
{
	//Make the VAO the current Vertex Array Object by bindint it
	glBindVertexArray(ID);
}
// Unbinds the VAO
void VAO::Unbind()
{
	glBindVertexArray(0); //unbinding the VAO by binding it to 0
}
// Deletes the VAO
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}