#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"       // Upewnij się, że ścieżki są poprawne
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "ModelHandling.h" // Tutaj jest zmodyfikowana wersja
#include <vector>
#include <ctime> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "objloader.h"

//dla szafki
std::vector<GLfloat> vertices;
std::vector<GLuint> indices;

std::vector<GLfloat> floor_vertices;
std::vector<GLuint> floor_indices;

std::vector<GLfloat> grass_vertices;
std::vector<GLuint> grass_indices;

std::vector<GLfloat> door_vertices;
std::vector<GLuint> door_indices;

std::vector<GLfloat> trash_vertices;
std::vector<GLuint> trash_indices;

std::vector<GLfloat> milk_vertices;
std::vector<GLuint> milk_indices;

std::vector<Vertex> lampVertices;
std::vector<GLuint> lampIndicesVec;

GLfloat lightVertices[] = {
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] = {
	0, 1, 2, 0, 2, 3, 0, 4, 7, 0, 7, 3, 3, 7, 6, 3, 6, 2,
	2, 6, 5, 2, 5, 1, 1, 5, 4, 1, 4, 0, 4, 5, 6, 4, 6, 7
};

GLfloat skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, 
     1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f 
};

int main()
{
	// -------------- WYMIARY SZAFKI --------------
	double x_cupboard = 0.0, y_cupboard = 0.0, z_cupboard = 0.0;
	double w = 2.0, h = 3.0, d = 1.0;
	double t = 0.1;
	double leg_h = 0.2;
	double door_w = w - 2 * t;
	double door_h = h - 2 * t;
	double door_thickness = t;

    glm::vec3 door_hinge_position = glm::vec3(
        static_cast<float>(x_cupboard + w-0.1),
        static_cast<float>(y_cupboard + t),
        static_cast<float>(z_cupboard + d - door_thickness)
    );

	// -------------- TWORZENIE MODELI --------------
	add_cube(floor_vertices, floor_indices, -2.0, -1.0, -2.0, 4.0 + w, 1.0 - leg_h, 4.0 + d, 1.0); //podstawka betonowa
	add_plane(grass_vertices, grass_indices, //trawa
              -100.0, -1.0, -100.0, 
              -100.0, -1.0,  100.0,
               100.0, -1.0,  100.0,  
               100.0, -1.0, -100.0,  
               0.0, 1.0, 0.0,      
               20.0);           

	// -------------- TWORZENIE SZAFKI --------------
	add_cube(vertices, indices, x_cupboard, y_cupboard, z_cupboard, w, t, d); 
	add_cube(vertices, indices, x_cupboard, y_cupboard + h - t, z_cupboard, w, t, d);
	add_cube(vertices, indices, x_cupboard, y_cupboard + t, z_cupboard, t, h - 2 * t, d);
	add_cube(vertices, indices, x_cupboard + w - t, y_cupboard + t, z_cupboard, t, h - 2 * t, d);
	add_cube(vertices, indices, x_cupboard + t, y_cupboard + t, z_cupboard, w - 2 * t, h - 2 * t, t);
	add_cube(vertices, indices, x_cupboard + t, y_cupboard + h / 2 - t / 2, z_cupboard + t, w - 2 * t, t, d - 2 * t);
	add_cube(vertices, indices, x_cupboard, y_cupboard - leg_h, z_cupboard + d - t, t, leg_h, t, 0.2);
	add_cube(vertices, indices, x_cupboard + w - t, y_cupboard - leg_h, z_cupboard + d - t, t, leg_h, t, 0.2);
	add_cube(vertices, indices, x_cupboard, y_cupboard - leg_h, z_cupboard, t, leg_h, t, 0.2);
	add_cube(vertices, indices, x_cupboard + w - t, y_cupboard - leg_h, z_cupboard, t, leg_h, t, 0.2);

	// -------------- TWORZENIE LOSOWE PIWA I MLEKA --------------
	srand(static_cast<unsigned>(time(0)));
	for (double i = 0.2; i < 1.6; i += 0.2) {
		float random_value = 0.1f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (0.3f)));
		add_cube(trash_vertices, trash_indices, 1.01*i, (h+0.1) / 2, random_value, 0.15, 0.35, 0.15);
	}
	for (double i = 0.2; i < 1.6; i += 0.2) {
		float random_value = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (0.3f)));
		add_cube(trash_vertices, trash_indices, 1.01 * i, (h + 0.1) / 2, random_value, 0.15, 0.35, 0.15);
	}

	for (double i = 0.2; i < 1.6; i += 0.2) {
		float random_value = 0.1f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (0.3f)));
		add_cube(milk_vertices, milk_indices, 1.01 * i, 0.1, random_value, 0.15, 0.4, 0.15);
	}
	for (double i = 0.2; i < 1.6; i += 0.2) {
		float random_value = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (0.3f)));
		add_cube(milk_vertices, milk_indices, 1.01 * i, 0.1, random_value, 0.15, 0.4, 0.15);
	}
	
	add_cube(door_vertices, door_indices,
		0.0,                      // lokalne x = 0 (tu będzie zawias)
		0.0,                      // lokalne y = 0
		0.0,                      // lokalne z = 0
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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
	glViewport(0, 0, 1000, 1000);

    // Stride dla danych wierzchołków (pozycja + normalna + texcoord)
    int stride = 8 * sizeof(GLfloat);

	Shader shaderProgram("default.vert", "default.frag");
	bool door_opened = false;
	bool animation_on = false;
	float door_angle = 0.0f;

	Shader doorShader("door.vert", "door.frag");

	// -------------- VAO DLA SZAFKI --------------
	VAO VAO_cupboard;
	VAO_cupboard.Bind();
	VBO VBO_cupboard(vertices.data(), vertices.size() * sizeof(GLfloat));
	EBO EBO_cupboard(indices.data(), indices.size() * sizeof(GLuint));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	VAO_cupboard.Unbind();

	// -------------- VAO DLA DRZWI --------------
	VAO VAO_door;
	VAO_door.Bind();
	VBO VBO_door(door_vertices.data(), door_vertices.size() * sizeof(GLfloat));
	EBO EBO_door(door_indices.data(), door_indices.size() * sizeof(GLuint));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	VAO_door.Unbind();

	// -------------- VAO DLA PODLOGI --------------
	VAO VAO_floor;
	VAO_floor.Bind();
	VBO VBO_floor(floor_vertices.data(), floor_vertices.size() * sizeof(GLfloat));
	EBO EBO_floor(floor_indices.data(), floor_indices.size() * sizeof(GLuint));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	VAO_floor.Unbind();

	// -------------- VAO DLA TRAWY --------------
	VAO VAO_grass;
	VAO_grass.Bind();
	VBO VBO_grass(grass_vertices.data(), grass_vertices.size() * sizeof(GLfloat));
	EBO EBO_grass(grass_indices.data(), grass_indices.size() * sizeof(GLuint));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	VAO_grass.Unbind();

	// -------------- VAO DLA PUSZEK --------------
	VAO VAO_trash;
	VAO_trash.Bind();
	VBO VBO_trash(trash_vertices.data(), trash_vertices.size() * sizeof(GLfloat));
	EBO EBO_trash(trash_indices.data(), trash_indices.size() * sizeof(GLuint));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	VAO_trash.Unbind();

	// -------------- VAO DLA MLEKA --------------
	VAO VAO_milk;
	VAO_milk.Bind();
	VBO VBO_milk(milk_vertices.data(), milk_vertices.size() * sizeof(GLfloat));
	EBO EBO_milk(milk_indices.data(), milk_indices.size() * sizeof(GLuint));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	VAO_milk.Unbind();

	// -------------- LAMPA LOAD --------------
	ObjLoader loader;
	const char* lampFile = "Street_Lamp.obj";
	loader.LoadObj(lampFile); 

	int lampVertCount = loader.numVertices();    
	int lampIndexCount = loader.numIndices();     

	const float* lampPositions = reinterpret_cast<const float*>(loader.getVertices());
	const float* lampNormals   = reinterpret_cast<const float*>(loader.getNormals());
	const float* lampTexCoords = reinterpret_cast<const float*>(loader.getCoords());
	const unsigned int* lampIndices = loader.getIndices();

	// std::cout<< lampVertCount << "	lampVertCount\n";
	// std::cout<< lampIndexCount << "	lampIndexCount\n";
	// std::cout<< *lampPositions << "	lampPositions\n";
	// std::cout<< *lampNormals << "	lampNormals\n";
	// std::cout<< *lampTexCoords << "	lampTexCoords\n";
	// std::cout<< *lampIndices << "	lampIndices\n";

	for (int i = 0; i < lampVertCount; ++i)
	{
    Vertex vertex;
    vertex.position = glm::vec3(
        lampPositions[i * 3 + 0],
        lampPositions[i * 3 + 1],
        lampPositions[i * 3 + 2]);

    vertex.normal = glm::vec3(
        lampNormals[i * 3 + 0],
        lampNormals[i * 3 + 1],
        lampNormals[i * 3 + 2]);

    vertex.color = glm::vec3(1.0f); // white or placeholder (OBJ doesn't store color)

    vertex.texUV = glm::vec2(
        lampTexCoords[i * 2 + 0],
        lampTexCoords[i * 2 + 1]);

    lampVertices.push_back(vertex);
	}
	// inicies copy
	lampIndicesVec.assign(lampIndices, lampIndices + lampIndexCount);
	// for (const Vertex& v : lampVertices)
    // std::cout << v.position.x << ", " << v.position.y << ", " << v.position.z << "\n";

	// -------------- SZADER --------------
	Shader lightShader("light.vert", "light.frag");
	VAO lightVAO;
	lightVAO.Bind();
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));
    // Dla sześcianu światła używamy tylko pozycji (location = 0)
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	// -------------- SHADERY I VAO/VBO DLA SKYBOXA --------------
	Shader skyboxShader("skybox.vert", "skybox.frag");
	VAO skyboxVAO;
	skyboxVAO.Bind();
	VBO skyboxVBO(skyboxVertices, sizeof(skyboxVertices));
	// Tylko pozycje dla skyboxa
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	skyboxVAO.Unbind();
	// -------------- ŁADOWANIE TEKSTURY CUBEMAP --------------
	std::vector<std::string> faces {
		"Textures/skybox/px.png",  
		"Textures/skybox/nx.png",
		"Textures/skybox/py.png",
		"Textures/skybox/ny.png",
		"Textures/skybox/pz.png",
		"Textures/skybox/nz.png"
	};

	GLuint cubemapTexture =Texture::loadCubemap(faces);
	if (cubemapTexture == 0) {
		std::cout << "Nie udało się załadować cubemapy!" << std::endl;
	}

	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	Camera camera(1000, 1000, glm::vec3(3.0f, 2.5f, 5.0f));
    camera.Orientation = glm::normalize(glm::vec3(x_cupboard + w/2, y_cupboard + h/2, z_cupboard + d/2) - camera.Position); // Kamera patrzy na szafkę

	glm::vec4 lightColorVal = glm::vec4(1.0f, 1.0f, 0.8f, 1.0f);
	glm::vec3 lightPosVal = glm::vec3(x_cupboard + w / 2.0f, y_cupboard + h + 1.0f, z_cupboard + d + 2.0f); // Światło nad szafką
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPosVal);

	shaderProgram.Activate();
	glUniform4fv(glGetUniformLocation(shaderProgram.ID, "lightColor"), 1, glm::value_ptr(lightColorVal));
	glUniform3fv(glGetUniformLocation(shaderProgram.ID, "lightPos"), 1, glm::value_ptr(lightPosVal));

	doorShader.Activate();
	glUniform4fv(glGetUniformLocation(doorShader.ID, "lightColor"), 1, glm::value_ptr(lightColorVal));
	glUniform3fv(glGetUniformLocation(doorShader.ID, "lightPos"), 1, glm::value_ptr(lightPosVal));

	Texture texture_wood("Textures/wood.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
	Texture texture_concrete("Textures/concrete.jpg", GL_TEXTURE_2D, 1, GL_RGB, GL_UNSIGNED_BYTE);
	Texture texture_grass("Textures/grass.jpg", GL_TEXTURE_2D, 2, GL_RGB, GL_UNSIGNED_BYTE);
	Texture texture_door("Textures/door.jpg", GL_TEXTURE_2D, 3, GL_RGB, GL_UNSIGNED_BYTE);
	Texture texture_beer("Textures/beer.jpg", GL_TEXTURE_2D, 4, GL_RGB, GL_UNSIGNED_BYTE);
	Texture texture_milk("Textures/milk.jpg", GL_TEXTURE_2D, 5, GL_RGB, GL_UNSIGNED_BYTE);

	// -------------- LAMP MESH --------------
	// Pusta
	std::vector<Texture> lampTextures;
	Texture blackTex("Textures/black.png", GL_TEXTURE_2D, 7, GL_RGBA, GL_UNSIGNED_BYTE);
	lampTextures.push_back(blackTex);
	assert(!lampVertices.empty() && "lampVertices is empty!");
	assert(!lampIndicesVec.empty() && "lampIndicesVec is empty!");
	// Meeeesh
	Mesh* lampMesh = new Mesh(lampVertices, lampIndicesVec, lampTextures);

    // Ustawienie jednostek tekstur dla samplerów w shaderach
    shaderProgram.Activate();
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "tex0"), 0); 

    doorShader.Activate();
    glUniform1i(glGetUniformLocation(doorShader.ID, "doorTexture"), 3); 

	glEnable(GL_DEPTH_TEST);
	float lastFrameTime = 0.0f;

	glfwSwapInterval(1); // vsync

	if (glGetUniformLocation(shaderProgram.ID, "tex0") == -1) {
		std::cout << "UWAGA: Nie ma tex0 w unofrm shaderze" << std::endl;
	}

	while (!glfwWindowShouldClose(window))
	{
		float currentFrameTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		camera.Inputs(window);
		// czy klawisz nacisniety, mozna zmienic na ta pierwsza linijke wtedy jak animacja jest w toku nie da sie jej odwrocic
		//if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !animation_on) {
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {

            // Prosty debounce, żeby jedno naciśnięcie nie odpaliło animacji wielokrotnie
            static double last_e_press = 0.0;
            if (currentFrameTime - last_e_press > 0.3) { // 0.3 sekundy przerwy
                animation_on = true;
                door_opened = !door_opened;
                last_e_press = currentFrameTime;
            }
		}

		if (animation_on) {
			float target_angle_deg = door_opened ? -90.0f : 0.0f;
			float angular_velocity_deg_s = 120.0f; // stopnie na sekundę

            float diff_angle = target_angle_deg - door_angle;
            float move_angle = angular_velocity_deg_s * deltaTime;

            if (std::abs(diff_angle) < std::abs(move_angle)) {
                door_angle = target_angle_deg;
                animation_on = false;
            } else {
                if (diff_angle < 0) { 
                    door_angle -= move_angle;
                } else { 
                    door_angle += move_angle;
                }
            }
		}

		camera.updateMatrix(45.0f, 0.01f, 100.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightShader.Activate();
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	    glUniform4fv(glGetUniformLocation(lightShader.ID, "lightColor"), 1, glm::value_ptr(lightColorVal));
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		shaderProgram.Activate();
		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "viewPos"), 1, glm::value_ptr(camera.Position));
		camera.Matrix(shaderProgram, "camMatrix");

        glm::mat4 identityModel = glm::mat4(1.0f); // Dla obiektów, które są już w world space
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(identityModel));

		// ---------------------------- rysowanie szafki ----------------------------
        glActiveTexture(GL_TEXTURE0); 
		texture_wood.Bind();    
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "tex0"), 0);
		VAO_cupboard.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

		// ---------------------------- rysowanie podlogi ----------------------------
		glActiveTexture(GL_TEXTURE1); 
		texture_concrete.Bind();      
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "tex0"), 1);
		VAO_floor.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(floor_indices.size()), GL_UNSIGNED_INT, 0);

		// ---------------------------- rysowanie trawy ----------------------------
		glActiveTexture(GL_TEXTURE2); 
		texture_grass.Bind();        
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "tex0"), 2);
		VAO_grass.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(grass_indices.size()), GL_UNSIGNED_INT, 0);

		// ---------------------------- rysowanie puszek ----------------------------
		glActiveTexture(GL_TEXTURE4);
		texture_beer.Bind();        
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "tex0"), 4); 
		VAO_trash.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(trash_indices.size()), GL_UNSIGNED_INT, 0);

		// ---------------------------- rysowanie mLeka ----------------------------
		glActiveTexture(GL_TEXTURE5);
		texture_milk.Bind();
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "tex0"), 5);
		VAO_milk.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(milk_indices.size()), GL_UNSIGNED_INT, 0);

        // Przywróć sampler shaderProgram do jednostki 0 dla następnej klatki, jeśli szafka ma być pierwsza
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "tex0"), 0);

		// ---------------------------- rysowanie drzwi ----------------------------
		doorShader.Activate();
		glUniform3fv(glGetUniformLocation(doorShader.ID, "viewPos"), 1, glm::value_ptr(camera.Position));
		camera.Matrix(doorShader, "camMatrix");

		glm::mat4 doorModelMatrix = glm::mat4(1.0f);
        doorModelMatrix = glm::translate(doorModelMatrix, door_hinge_position);
		doorModelMatrix = glm::rotate(doorModelMatrix, glm::radians(door_angle), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(glGetUniformLocation(doorShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(doorModelMatrix));

		glActiveTexture(GL_TEXTURE3); // Aktywuj jednostkę 3
		texture_door.Bind();          // Powiąż teksturę drzwi (na jednostce 3)
		VAO_door.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(door_indices.size()), GL_UNSIGNED_INT, 0);

		// ---------------------------- rysowanie lampy ----------------------------
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		glm::mat4 lampModel = glm::mat4(1.0f);
		lampModel = glm::translate(lampModel, glm::vec3(0.0f, 0.0f, 0.0f));
		lampModel = glm::scale(lampModel, glm::vec3(0.02f));  // Optional scale

		lampMesh->Draw(shaderProgram, camera, lampModel);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// ----------------------------- RYSOWANIE SKYBOXA (na końcu) -----------------------------
		glDepthFunc(GL_LEQUAL);
		// bo ustawiamy gl_equal, z=w, co daje głębokość 1.0
		skyboxShader.Activate();
		// Przekaż macierze projekcji i widoku (bez translacji)
		glm::mat4 view = camera.getViewMatrix(); // Pobierz macierz widoku z kamery
		glm::mat4 projection = camera.getProjectionMatrix(45.0f, 0.01f, 100.0f); // Pobierz macierz projekcji
		// (parametry muszą być takie same jak w camera.updateMatrix)

		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		skyboxVAO.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		skyboxVAO.Unbind();
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		glDepthFunc(GL_LESS);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// -------------- USUWANIE WSZYSTKIEGO --------------

	delete lampMesh; 
	VAO_cupboard.Delete(); VBO_cupboard.Delete(); EBO_cupboard.Delete(); texture_wood.Delete();
	VAO_floor.Delete(); VBO_floor.Delete(); EBO_floor.Delete(); texture_concrete.Delete();
    VAO_grass.Delete(); VBO_grass.Delete(); EBO_grass.Delete(); texture_grass.Delete();
    VAO_door.Delete(); VBO_door.Delete(); EBO_door.Delete(); texture_door.Delete();
	lightVAO.Delete(); lightVBO.Delete(); lightEBO.Delete();
	glDeleteTextures(1, &cubemapTexture);
	skyboxVAO.Delete();
	skyboxShader.Delete();
	lightShader.Delete();
	shaderProgram.Delete();
    doorShader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}