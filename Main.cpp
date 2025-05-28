#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"camera.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"
#include"ModelHandling.h"
#include <vector>

std::vector<GLfloat> vertices;
std::vector<GLuint> indices;

//GLfloat vertices[] = {
//	// TEX - kordy tekstury
//	// Back left face (z = -0.5)
//	//X		Y		Z				TEX.U TEX.V
//	-1.5f,  0.5f, -0.5f,			0.0f, 0.0f, // 8: 
//	-0.5f,  0.5f, -0.5f,			0.0f, 1.0f, // 9: 
//	-1.5f, -0.5f, -0.5f,			1.0f, 1.0f, // 10: 
//	-0.5f, -0.5f, -0.5f,			1.0f, 0.0f, // 11: 
//
//	// Back right face (z = -0.5)
//	//X		Y		Z				TEX.U TEX.V
//	 0.5f,  0.5f, -0.5f,			0.0f, 0.0f, // 12: 
//	 1.5f,  0.5f, -0.5f,			0.0f, 1.0f, // 13: 
//	 0.5f, -0.5f, -0.5f,			1.0f, 1.0f, // 14: 
//	 1.5f, -0.5f, -0.5f,			1.0f, 0.0f, // 15: 
//	// Front left face (z = 0.5)
//	//X		Y		Z				TEX.U TEX.V
//	-1.5f,  0.5f,  0.5f,			0.0f, 0.0f, // 0: 
//	-0.5f,  0.5f,  0.5f,			0.0f, 1.0f, // 1: 
//	-1.5f, -0.5f,  0.5f,			1.0f, 1.0f, // 2: 
//	-0.5f, -0.5f,  0.5f,			1.0f, 0.0f, // 3: 
//
//	// Front right face (z = 0.5)
//	//X		Y		Z				TEX.U TEX.V
//	 0.5f,  0.5f, 0.5f,				1.0f, 0.0f, // 4: 
//	 1.5f,  0.5f, 0.5f,				1.0f, 1.0f, // 5:
//	 0.5f, -0.5f, 0.5f,				0.0f, 1.0f, // 6: 
//	 1.5f, -0.5f, 0.5f,				0.0f, 0.0f  // 7: 
//};

//GLuint indices[] = {
//	// Left cube 
//	// Front face
//	0, 1, 2,
//	1, 2, 3,
//	// Back face
//	8, 9, 10,
//	9, 10, 11,
//	// Left face
//	0, 2, 8,
//	2, 8, 10,
//	// Top face
//	0, 1, 8,
//	1, 8, 9,
//	// Bottom face
//	2, 3, 10,
//	3, 10, 11,
//
//	// Middle cube 
//	// Top face
//	1, 4, 9,
//	4, 9, 12,
//	// Bottom face
//	3, 6, 11,
//	6, 11, 14,
//
//	// Right cube
//	// Front face
//	4, 5, 6,
//	5, 6, 7,
//	// Back face
//	12, 13, 14,
//	13, 14, 15,
//	// Right face
//	5, 7, 13,
//	7, 13, 15,
//	// Top face
//	4, 5, 12,
//	5, 12, 13,
//	// Bottom face
//	6, 7, 14,
//	7, 14, 15
//};

int main()
{
	add_cube(vertices, indices, 0, 0, 0, 1, 1, 0.5);
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 800, "YoutubeOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, 800, 800);

	Shader shaderProgram("default.vert", "default.frag");

	VBO VBO1(vertices.data(), vertices.size() * sizeof(GLfloat));
	EBO EBO1(indices.data(), indices.size() * sizeof(GLuint));

	VAO VAO1;
	VAO1.Bind();
	VBO1.Bind();
	EBO1.Bind();

	// Now define the vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind all
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	Camera camera(800, 800, glm::vec3(0.0f, 0.0f, 3.0f));

	Texture texture1("Textures/rauch.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture texture2("Textures/dziekaners.jpg", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);

	shaderProgram.Activate();

	texture1.texUnit(shaderProgram, "tex0", 0);
	texture2.texUnit(shaderProgram, "tex1", 1);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		// Input
		camera.Inputs(window);
		glfwPollEvents();

		// Rendering
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		// Aktywuj obie tekstury
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1.ID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2.ID);

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}