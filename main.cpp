#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>
#include "shaderClass.h"
#include <stb/stb_image.h>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
using std::cout;
using std::endl;

GLfloat vertices[] =
{
//        COORDINATES                 /    COLORS 
	-0.5f, -0.5f, 0.0f,      1.0f,  0.0f, 0.0f,    0.0f, 0.0f, //Lower Left Corner
	-0.5f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f,  0.0f, 1.0f, //Upper Left Corner
	0.5f, 0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f, //Upper Right Corner
	0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f //Lower Left Corner
};
GLuint indices[] =
{
	0, 2, 1, //Upper Triangle
	0, 3, 2 //Lower Triangle 
};

int main()
{
	//To tell the user the program is starting
	cout << "Starting program" << endl << endl;
	//To initialize glfw
	glfwInit();
	//To tell glfw which version we are using
	//We are using glfw 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//To tell glfw we are using the core profile
	//We have access to only modern functions using the core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//To create a glfw window that has a width of 800 pixels and a height of 800 pixels
	GLFWwindow* window = glfwCreateWindow(800, 800, "Program", NULL, NULL);
	//To check if the window fails to load
	if (window == NULL)
	{
		cout << "Failed to open window" << endl;
		glfwTerminate();
		return -1;
	}
	//To introduce window into current context
	glfwMakeContextCurrent(window);
	//To load glad so it configures OpenGL
	gladLoadGL();
	//To specify the viewport of OpenGL in thw window
	//In this case the viewport goes from x = 0, y = 0 to x = 800, y = 800
	glViewport(0, 0, 800, 800);

	//To generate Shader Object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	//To generate Vertex Array Object and bind it
	VAO VAO1;
	VAO1.Bind();
	//To generate Vertex Buffer Object and link it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	//To generate Element Buffer Object and link it to indices
	EBO EBO1(indices, sizeof(indices));
	//To link VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//To unbind all objects to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	//Gets ID of uniform called 'scale'
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	//Texture
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load("beluga.jpg", &widthImg, &heightImg, &numColCh, 0);
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//  float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f}
//  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(tex0Uni, 0);
	//Main while loop
	while(!glfwWindowShouldClose(window))
	{
		//To specify the color of the background
		glClearColor(0.0f, 1.0f, 0.80f, 1.0f);
		//To clean the back buffer and assign a new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		//To tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		//To assign a value to the uniform. Must always be done after activating the Shader Program
		glUniform1f(uniID, 0.5f);
		glBindTexture(GL_TEXTURE_2D, texture);
		//To bind the VAO so OpenGL knows how to use it
		VAO1.Bind();
		//To draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//To swap the front and back buffer
		glfwSwapBuffers(window);
		//To take care of all glfw events
		glfwPollEvents();
	}
	//To delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	glDeleteTextures(1, &texture);
	shaderProgram.Delete();
	//To close window before ending program
	glfwDestroyWindow(window);
	//To tell the user they ended the program
	cout << endl << "Ending program" << endl;
	//To terminte glfw before ending the program
	glfwTerminate();
	return 0;
}