#include "VAO.h"

VAO::VAO()
{
	//To generate the VAO with only one object
	glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	//To configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	//To enable the Vertex Attribute so that OpenGL knows how to use it
	glEnableVertexAttribArray(layout);
	VBO.Unbind();

}

void VAO::Bind()
{
	//To make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(ID);
}
void VAO::Unbind()
{
	//To unbind the VAO
	glBindVertexArray(0);
}
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}


