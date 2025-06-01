#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <string>
#include <cassert>

struct Vec3 {
    float x, y, z;
    float operator[](int idx) const {
        assert(idx >= 0 && idx < 3);
        return idx == 0 ? x : (idx == 1 ? y : z);
    }
};

struct TexCoord {
    float u, v;
    float operator[](int idx) const {
        assert(idx >= 0 && idx < 2);
        return idx == 0 ? u : v;
    }
};

struct Triangle {
    int pos = -1, norm = -1, coord = -1;
};

class ObjLoader {
public:
    bool LoadObj(const std::string& path);

    const Vec3* getVertices() const;
    const Vec3* getNormals() const;
    const TexCoord* getCoords() const;
    const unsigned int* getIndices() const;

    size_t numVertices() const;
    size_t numNormals() const;
    size_t numCoords() const;
    size_t numIndices() const;

private:
    std::vector<Vec3> Vertices;
    std::vector<Vec3> Normals;
    std::vector<TexCoord> Coords;
    std::vector<unsigned int> Indices;
};

#endif
