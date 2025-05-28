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

std::vector<GLfloat> floor_vertices;
std::vector<GLuint> floor_indices;

std::vector<GLfloat> grass_vertices;
std::vector<GLuint> grass_indices;

int main()
{
	// -------------- WYMIARY SZAFKI --------------
	double x = 0.0, y = 0.0, z = 0.0;
	double w = 2.0, h = 3.0, d = 1.0;
	double t = 0.1;
	double leg_h = 0.2;
	double door_w = w - 2 * t;
	double door_h = h - 2 * t;
	double door_thickness = t;

	// -------------- TWORZENIE MODELI --------------
	add_cube(floor_vertices, floor_indices, -2, -1, -2, 4+w, 0.8, 4+d);

	add_plane(grass_vertices, grass_indices, -100,-0.8,-100,  -100, -0.8,100,    100, -0.8,100,    100, -0.8,-100,    20);

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

	// -------------- GLFW --------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1000, 1000, "OpenSzafkaGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, 1000, 1000);

	Shader shaderProgram("default.vert", "default.frag");

	// -------------- VAO DLA SZAFKI --------------
	VAO VAO_cupboard;
	VAO_cupboard.Bind();

	VBO VBO_cupboard(vertices.data(), vertices.size() * sizeof(GLfloat));
	EBO EBO_cupboard(indices.data(), indices.size() * sizeof(GLuint));

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	VAO_cupboard.Unbind();

	// -------------- VAO DLA PODLOGI --------------

	VAO VAO_floor;
	VAO_floor.Bind();

	VBO VBO_floor(floor_vertices.data(), floor_vertices.size() * sizeof(GLfloat));
	EBO EBO_floor(floor_indices.data(), floor_indices.size() * sizeof(GLuint));

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	VAO_floor.Unbind();

	// -------------- VAO DLA TRAWY --------------

	VAO VAO_grass;
	VAO_grass.Bind();

	VBO VBO_grass(grass_vertices.data(), grass_vertices.size() * sizeof(GLfloat));
	EBO EBO_grass(grass_indices.data(), grass_indices.size() * sizeof(GLuint));

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	VAO_grass.Unbind();

	// -------------- CAMERA --------------

	Camera camera(800, 800, glm::vec3(1.5f, 2.0f, 10.0f));

	Texture texture1("Textures/wood.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture texture2("Textures/concrete.jpg", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture texture3("Textures/grass.jpg", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);

	shaderProgram.Activate();

	texture1.texUnit(shaderProgram, "tex0", 0);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		// Input
		camera.Inputs(window);
		glfwPollEvents();

		// Rendering
		glClearColor(0.5f, 0.7f, 1.0f, 1.0f); //kolor t³a
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		// -------------- rysowanie szafki --------------
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1.ID);

		VAO_cupboard.Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		// -------------- rysowanie podlogi --------------
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2.ID);

		VAO_floor.Bind();
		glDrawElements(GL_TRIANGLES, floor_indices.size(), GL_UNSIGNED_INT, 0);

		// -------------- rysowanie trawy --------------
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3.ID);

		VAO_grass.Bind();
		glDrawElements(GL_TRIANGLES, grass_indices.size(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}

	VAO_cupboard.Delete();
	VBO_cupboard.Delete();
	EBO_cupboard.Delete();

	VAO_floor.Delete();
	VBO_floor.Delete();
	EBO_floor.Delete();

	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}