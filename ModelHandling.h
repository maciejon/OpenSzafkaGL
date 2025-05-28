#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"camera.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"
#include <vector>
using namespace std;
void add_plane(vector<GLfloat>& vrt, vector<GLuint>& ind, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double x4, double y4, double z4, double tex_size=1);
void add_cube(vector<GLfloat>& vrt, vector<GLuint>& ind, double x, double y, double z, double a, double b, double c);