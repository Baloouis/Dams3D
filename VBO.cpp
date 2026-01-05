#include"VBO.h"

// Constructor that generates a Vertex Buffer Object and links it to vertices. 
// A VBO is used to store data on the vertices ( not just position but also additional info, such as color or texture coordinates, etc ...)
VBO::VBO(std::vector<Vertex>& vertices)
{
	//(creating the reference container for the Vertex Buffer Object)
	//Generate the VBO with only 1 object
	glGenBuffers(1, &ID);
	//Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	//Link our vertices data into the VBO
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

// Binds the VBO
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}
// Unbinds the VBO
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinding the VBO by binding it to 0
}
// Deletes the VBO
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}