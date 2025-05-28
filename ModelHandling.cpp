#include <iostream>
#include <vector>
#include "ModelHandling.h"
using namespace std;

//funkcja ktora dodaje do verticesow i indicesow prostokat. x1,y1,z1 to pierwszy wierzcholek itd.
void add_plane(vector<GLfloat>& vrt, vector<GLuint>& ind, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double x4, double y4, double z4, double tex_size) {

    GLuint start = static_cast<GLuint>(vrt.size() / 5);
    GLfloat texs = static_cast<GLfloat>(tex_size);

	vrt.push_back(x1); vrt.push_back(y1); vrt.push_back(z1); vrt.push_back(0.0f); vrt.push_back(0.0f);
	vrt.push_back(x2); vrt.push_back(y2); vrt.push_back(z2); vrt.push_back(0.0f); vrt.push_back(texs);
	vrt.push_back(x3); vrt.push_back(y3); vrt.push_back(z3); vrt.push_back(texs); vrt.push_back(texs);
	vrt.push_back(x4); vrt.push_back(y4); vrt.push_back(z4); vrt.push_back(texs); vrt.push_back(0.0f);


	ind.push_back(start);
	ind.push_back(start + 1);
	ind.push_back(start + 2);

	ind.push_back(start);
	ind.push_back(start + 2);
	ind.push_back(start + 3);
}

//funkcja ktora dodaje prostopadloscian a wcale nie cube
//aby stworzyc fajna scianke trzeba a, b lub c ustawic na mala wartosc
void add_cube(std::vector<GLfloat>& vrt, std::vector<GLuint>& ind, double x, double y, double z, double a, double b, double c) {
    // Front face (+Z)
    add_plane(vrt, ind,
        x, y, z + c,
        x + a, y, z + c,
        x + a, y + b, z + c,
        x, y + b, z + c, (a+b+c)/3);

    // Back face (-Z)
    add_plane(vrt, ind,
        x + a, y, z,
        x, y, z,
        x, y + b, z,
        x + a, y + b, z, (a + b + c) / 3);

    // Left face (-X)
    add_plane(vrt, ind,
        x, y, z,
        x, y, z + c,
        x, y + b, z + c,
        x, y + b, z, (a + b + c) / 3);

    // Right face (+X)
    add_plane(vrt, ind,
        x + a, y, z + c,
        x + a, y, z,
        x + a, y + b, z,
        x + a, y + b, z + c, (a + b + c) / 3);

    // Top face (+Y)
    add_plane(vrt, ind,
        x, y + b, z + c,
        x + a, y + b, z + c,
        x + a, y + b, z,
        x, y + b, z, (a + b + c) / 3);

    // Bottom face (-Y)
    add_plane(vrt, ind,
        x, y, z,
        x + a, y, z,
        x + a, y, z + c,
        x, y, z + c, (a + b + c) / 3);
}

