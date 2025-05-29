#include <vector>
#include <cmath> // Dla sqrt, jeśli będziesz potrzebował normalizacji
#include "ModelHandling.h" // Zakładam, że tu są definicje GLfloat i GLuint

// Jeśli nie używasz GLM, proste struktury i funkcje pomocnicze mogą być przydatne
// dla operacji wektorowych, np. obliczania normalnych.
// Ale dla prostopadłościanu, normalne są stałe dla każdej ściany.

// Funkcja pomocnicza do dodawania wierzchołka z pozycją, normalną i współrzędnymi tekstury
void add_vertex(std::vector<GLfloat>& vrt,
                GLfloat x, GLfloat y, GLfloat z,
                GLfloat nx, GLfloat ny, GLfloat nz,
                GLfloat u, GLfloat v) {
    vrt.push_back(x); vrt.push_back(y); vrt.push_back(z);
    vrt.push_back(nx); vrt.push_back(ny); vrt.push_back(nz);
    vrt.push_back(u); vrt.push_back(v);
}

// ZMODYFIKOWANA: add_plane teraz przyjmuje normalne jako argumenty.
// Zachowuje oryginalną nazwę, ale sygnatura jest rozszerzona o normalne.
void add_plane(std::vector<GLfloat>& vrt, std::vector<GLuint>& ind,
               double x1, double y1, double z1,
               double x2, double y2, double z2,
               double x3, double y3, double z3,
               double x4, double y4, double z4,
               double nx_val, double ny_val, double nz_val, // Dodane normalne
               double tex_size) {

    // Konwersja na GLfloat (ważne dla spójności typów w OpenGL)
    GLfloat gl_x1 = static_cast<GLfloat>(x1), gl_y1 = static_cast<GLfloat>(y1), gl_z1 = static_cast<GLfloat>(z1);
    GLfloat gl_x2 = static_cast<GLfloat>(x2), gl_y2 = static_cast<GLfloat>(y2), gl_z2 = static_cast<GLfloat>(z2);
    GLfloat gl_x3 = static_cast<GLfloat>(x3), gl_y3 = static_cast<GLfloat>(y3), gl_z3 = static_cast<GLfloat>(z3);
    GLfloat gl_x4 = static_cast<GLfloat>(x4), gl_y4 = static_cast<GLfloat>(y4), gl_z4 = static_cast<GLfloat>(z4);
    GLfloat gl_nx = static_cast<GLfloat>(nx_val), gl_ny = static_cast<GLfloat>(ny_val), gl_nz = static_cast<GLfloat>(nz_val);
    GLfloat gl_texs = static_cast<GLfloat>(tex_size);

    GLuint start_index = static_cast<GLuint>(vrt.size() / 8); // 3 poz + 3 norm + 2 tex = 8 floatów na wierzchołek

    // Wierzchołek 1 (lewy dolny dla typowej płaszczyzny XY patrząc w Z)
    add_vertex(vrt, gl_x1, gl_y1, gl_z1, gl_nx, gl_ny, gl_nz, 0.0f, 0.0f);
    // Wierzchołek 2 (lewy górny)
    add_vertex(vrt, gl_x2, gl_y2, gl_z2, gl_nx, gl_ny, gl_nz, 0.0f, gl_texs);
    // Wierzchołek 3 (prawy górny)
    add_vertex(vrt, gl_x3, gl_y3, gl_z3, gl_nx, gl_ny, gl_nz, gl_texs, gl_texs);
    // Wierzchołek 4 (prawy dolny)
    add_vertex(vrt, gl_x4, gl_y4, gl_z4, gl_nx, gl_ny, gl_nz, gl_texs, 0.0f);

    ind.push_back(start_index + 0);
    ind.push_back(start_index + 1);
    ind.push_back(start_index + 2);

    ind.push_back(start_index + 0);
    ind.push_back(start_index + 2);
    ind.push_back(start_index + 3);
}


// ZMODYFIKOWANA: add_cube używa wewnętrznie add_plane z przekazywaniem odpowiednich normalnych.
// Sygnatura tej funkcji pozostaje taka sama jak w oryginalnym kodzie.
void add_cube(std::vector<GLfloat>& vrt, std::vector<GLuint>& ind,
              double x, double y, double z,
              double a, double b, double c,
              double tex_size) {

    // Front face (+Z), Normal (0, 0, 1)
    add_plane(vrt, ind,
        x,     y,     z + c,  // Lewy dolny
        x,     y + b, z + c,  // Lewy górny
        x + a, y + b, z + c,  // Prawy górny
        x + a, y,     z + c,  // Prawy dolny
        0.0, 0.0, 1.0, tex_size);

    // Back face (-Z), Normal (0, 0, -1)
    // Kolejność wierzchołków jest ważna dla zachowania orientacji "na zewnątrz"
    // i spójnego mapowania tekstur (np. (0,0) w lewym dolnym rogu ściany).
    add_plane(vrt, ind,
        x + a, y,     z,      // Prawy dolny (patrząc od przodu, to jest "daleki" róg)
        x + a, y + b, z,      // Prawy górny
        x,     y + b, z,      // Lewy górny
        x,     y,     z,      // Lewy dolny
        0.0, 0.0, -1.0, tex_size);

    // Left face (-X), Normal (-1, 0, 0)
    add_plane(vrt, ind,
        x, y,     z,          // Dolny "bliski"
        x, y + b, z,          // Górny "bliski"
        x, y + b, z + c,      // Górny "daleki"
        x, y,     z + c,      // Dolny "daleki"
        -1.0, 0.0, 0.0, tex_size);

    // Right face (+X), Normal (1, 0, 0)
    add_plane(vrt, ind,
        x + a, y,     z + c,  // Dolny "daleki"
        x + a, y + b, z + c,  // Górny "daleki"
        x + a, y + b, z,      // Górny "bliski"
        x + a, y,     z,      // Dolny "bliski"
        1.0, 0.0, 0.0, tex_size);

    // Top face (+Y), Normal (0, 1, 0)
    add_plane(vrt, ind,
        x,     y + b, z + c,  // Lewy "daleki"
        x,     y + b, z,      // Lewy "bliski"
        x + a, y + b, z,      // Prawy "bliski"
        x + a, y + b, z + c,  // Prawy "daleki"
        0.0, 1.0, 0.0, tex_size);

    // Bottom face (-Y), Normal (0, -1, 0)
    add_plane(vrt, ind,
        x,     y,     z,      // Lewy "bliski"
        x,     y,     z + c,  // Lewy "daleki"
        x + a, y,     z + c,  // Prawy "daleki"
        x + a, y,     z,      // Prawy "bliski"
        0.0, -1.0, 0.0, tex_size);
}