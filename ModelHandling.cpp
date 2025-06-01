#include <vector>
#include <cmath>
#include "ModelHandling.h"

void add_vertex(std::vector<GLfloat>& vrt,
                GLfloat x, GLfloat y, GLfloat z,
                GLfloat nx, GLfloat ny, GLfloat nz,
                GLfloat u, GLfloat v) {
    vrt.push_back(x); vrt.push_back(y); vrt.push_back(z);
    vrt.push_back(nx); vrt.push_back(ny); vrt.push_back(nz);
    vrt.push_back(u); vrt.push_back(v);
}

void add_plane(std::vector<GLfloat>& vrt, std::vector<GLuint>& ind,
               double x1, double y1, double z1,
               double x2, double y2, double z2,
               double x3, double y3, double z3,
               double x4, double y4, double z4,
               double nx_val, double ny_val, double nz_val, 
               double tex_size) {

    GLfloat gl_x1 = static_cast<GLfloat>(x1), gl_y1 = static_cast<GLfloat>(y1), gl_z1 = static_cast<GLfloat>(z1);
    GLfloat gl_x2 = static_cast<GLfloat>(x2), gl_y2 = static_cast<GLfloat>(y2), gl_z2 = static_cast<GLfloat>(z2);
    GLfloat gl_x3 = static_cast<GLfloat>(x3), gl_y3 = static_cast<GLfloat>(y3), gl_z3 = static_cast<GLfloat>(z3);
    GLfloat gl_x4 = static_cast<GLfloat>(x4), gl_y4 = static_cast<GLfloat>(y4), gl_z4 = static_cast<GLfloat>(z4);
    GLfloat gl_nx = static_cast<GLfloat>(nx_val), gl_ny = static_cast<GLfloat>(ny_val), gl_nz = static_cast<GLfloat>(nz_val);
    GLfloat gl_texs = static_cast<GLfloat>(tex_size);

    GLuint start_index = static_cast<GLuint>(vrt.size() / 8);

    add_vertex(vrt, gl_x1, gl_y1, gl_z1, gl_nx, gl_ny, gl_nz, 0.0f, 0.0f);
    add_vertex(vrt, gl_x2, gl_y2, gl_z2, gl_nx, gl_ny, gl_nz, 0.0f, gl_texs);
    add_vertex(vrt, gl_x3, gl_y3, gl_z3, gl_nx, gl_ny, gl_nz, gl_texs, gl_texs);
    add_vertex(vrt, gl_x4, gl_y4, gl_z4, gl_nx, gl_ny, gl_nz, gl_texs, 0.0f);

    ind.push_back(start_index + 0);
    ind.push_back(start_index + 1);
    ind.push_back(start_index + 2);

    ind.push_back(start_index + 0);
    ind.push_back(start_index + 2);
    ind.push_back(start_index + 3);
}

// dodaje prostopadloscian, xyz - wspolrzedne jednego wierzcholka, abc - dlugosci bokow, tex_size - rozmiar tekstury
void add_cube(std::vector<GLfloat>& vrt, std::vector<GLuint>& ind,
              double x, double y, double z,
              double a, double b, double c,
              double tex_size) {

    // przód (+Z)
    add_plane(vrt, ind,
        x,     y,     z + c, 
        x,     y + b, z + c,  
        x + a, y + b, z + c, 
        x + a, y,     z + c,  
        0.0, 0.0, 1.0, tex_size);

    // tył (-Z)
    add_plane(vrt, ind,
        x + a, y,     z,   
        x + a, y + b, z,     
        x,     y + b, z,     
        x,     y,     z,  
        0.0, 0.0, -1.0, tex_size);

    // lewo (-X)
    add_plane(vrt, ind,
        x, y,     z,         
        x, y + b, z,       
        x, y + b, z + c,      
        x, y,     z + c,     
        -1.0, 0.0, 0.0, tex_size);

    // prawo (+X)
    add_plane(vrt, ind,
        x + a, y,     z + c, 
        x + a, y + b, z + c,  
        x + a, y + b, z,      
        x + a, y,     z,     
        1.0, 0.0, 0.0, tex_size);

    // góra (+Y)
    add_plane(vrt, ind,
        x,     y + b, z + c,
        x,     y + b, z,     
        x + a, y + b, z,    
        x + a, y + b, z + c, 
        0.0, 1.0, 0.0, tex_size);

    // dół (-Y)
    add_plane(vrt, ind,
        x,     y,     z,     
        x,     y,     z + c,
        x + a, y,     z + c, 
        x + a, y,     z,     
        0.0, -1.0, 0.0, tex_size);
}