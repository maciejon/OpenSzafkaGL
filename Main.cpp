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

int main()
{
	double x = 0.0, y = 0.0, z = 0.0;
	double w = 2.0, h = 3.0, d = 1.0;
	double t = 0.1;
	double leg_h = 0.2;
	double door_w = w - 2 * t;
	double door_h = h - 2 * t;
	double door_thickness = t;

	add_plane(vertices, indices, -50, -leg_h, -50, -50, -leg_h, 50, 50, -leg_h, 50, 50, -leg_h, -50, 5);
	add_cube(vertices, indices, x, y, z, w, t, d);
	add_cube(vertices, indices, x, y + h - t, z, w, t, d);
	add_cube(vertices, indices, x, y + t, z, t, h - 2 * t, d);
	add_cube(vertices, indices, x + w - t, y + t, z, t, h - 2 * t, d);
	add_cube(vertices, indices, x + t, y + t, z, w - 2 * t, h - 2 * t, t);
	add_cube(vertices, indices, x + t, y + h / 2 - t / 2, z + t, w - 2 * t, t, d - 2 * t);
	add_cube(vertices, indices, x, y - leg_h, z + d - t, t, leg_h, t);
	add_cube(vertices, indices, x + w - t, y - leg_h, z + d - t, t, leg_h, t);
	add_cube(vertices, indices, x, y - leg_h, z, t, leg_h, t);
	add_cube(vertices, indices, x + w - t, y - leg_h, z, t, leg_h, t);
	
	add_cube(vertices, indices,
		x + w,    
		y + t,                        
		z + d - door_thickness,      
		door_thickness,               
		door_h,                       
		door_w);          

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

	Texture texture1("Textures/herringbone.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
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
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

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