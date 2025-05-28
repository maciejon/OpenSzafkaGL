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

GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};
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
	//---------------Obsluga drzwi-----------
	bool drzwi_otwarte = false;
	bool animacja_w_toku = false;
	float kat_drzwi = 0.0f;
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
	//--------------- Oswietlenie-----------
	Shader lightShader("light.vert", "light.frag");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// -------------- CAMERA --------------

	Camera camera(800, 800, glm::vec3(1.5f, 2.0f, 10.0f));

	Texture texture1("Textures/wood.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture texture2("Textures/concrete.jpg", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture texture3("Textures/grass.jpg", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);

	texture1.texUnit(shaderProgram, "tex0", 0);
	glEnable(GL_DEPTH_TEST);

	float lastFrameTime = 0.0f;



	while (!glfwWindowShouldClose(window))
	{
		// Input
		camera.Inputs(window);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !animacja_w_toku) {
			animacja_w_toku = true;
			drzwi_otwarte = !drzwi_otwarte;
		}
		// Dzwi ruszanie - poczatek
		float currentFrameTime = glfwGetTime();           // aktualny czas
		float deltaTime = currentFrameTime - lastFrameTime; // różnica czasu (sekundy)
		lastFrameTime = currentFrameTime;
		if (animacja_w_toku) {
			float docelowy_kat = drzwi_otwarte ? 90.0f : 0.0f;
			float predkosc = 90.0f * deltaTime; // 90 stopni na sekundę

			if (drzwi_otwarte && kat_drzwi < docelowy_kat) {
				kat_drzwi += predkosc;
				if (kat_drzwi >= docelowy_kat) {
					kat_drzwi = docelowy_kat;
					animacja_w_toku = false;
				}
			} else if (!drzwi_otwarte && kat_drzwi > docelowy_kat) {
				kat_drzwi -= predkosc;
				if (kat_drzwi <= docelowy_kat) {
					kat_drzwi = docelowy_kat;
					animacja_w_toku = false;
				}
			}
		}
		glfwPollEvents();

		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		// Rendering
		glClearColor(0.5f, 0.7f, 1.0f, 1.0f); //kolor tla
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();
		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		// Export the camMatrix to the Vertex Shader of the pyramid
		camera.Matrix(shaderProgram, "camMatrix");

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

		// Tells OpenGL which Shader Program we want to use
		lightShader.Activate();
		// Export the camMatrix to the Vertex Shader of the light cube
		camera.Matrix(lightShader, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
		lightVAO.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO_cupboard.Delete();
	VBO_cupboard.Delete();
	EBO_cupboard.Delete();

	VAO_floor.Delete();
	VBO_floor.Delete();
	EBO_floor.Delete();

	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}