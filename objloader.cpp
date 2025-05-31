#include "objloader.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool ObjLoader::LoadObj(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Cannot open OBJ file: " << path << '\n';
        return false;
    }

    Vertices.clear();
    Normals.clear();
    Coords.clear();
    Indices.clear();

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            Vec3 v;
            iss >> v.x >> v.y >> v.z;
            Vertices.push_back(v);
        } else if (type == "vn") {
            Vec3 n;
            iss >> n.x >> n.y >> n.z;
            Normals.push_back(n);
        } else if (type == "vt") {
            TexCoord tc;
            iss >> tc.u >> tc.v;
            Coords.push_back(tc);
        } else if (type == "f") {
            std::vector<Triangle> faceVerts;
            std::string token;
            while (iss >> token) {
                std::istringstream tokenStream(token);
                std::string vStr, tStr, nStr;

                Triangle tri;

                size_t firstSlash = token.find('/');
                size_t secondSlash = token.find('/', firstSlash + 1);

                vStr = token.substr(0, firstSlash);
                if (firstSlash != std::string::npos && secondSlash != std::string::npos) {
                    tStr = token.substr(firstSlash + 1, secondSlash - firstSlash - 1);
                    nStr = token.substr(secondSlash + 1);
                } else if (firstSlash != std::string::npos) {
                    tStr = token.substr(firstSlash + 1);
                }

                auto parseIndex = [](const std::string& s, int count) -> int {
                    if (s.empty()) return -1;
                    int idx = std::stoi(s);
                    // Optional: enable negative index handling like OBJ spec
                    // return idx > 0 ? idx - 1 : count + idx;
                    return idx - 1;
                };

                tri.pos = parseIndex(vStr, Vertices.size());
                tri.coord = parseIndex(tStr, Coords.size());
                tri.norm = parseIndex(nStr, Normals.size());

                faceVerts.push_back(tri);
            }

            // Triangulate face (assumed convex polygon)
            for (size_t i = 1; i + 1 < faceVerts.size(); ++i) {
                Indices.push_back(faceVerts[0].pos);
                Indices.push_back(faceVerts[i].pos);
                Indices.push_back(faceVerts[i + 1].pos);
            }
        }
    }

    return true;
}

const Vec3* ObjLoader::getVertices() const {
    return Vertices.data();
}

const Vec3* ObjLoader::getNormals() const {
    return Normals.data();
}

const TexCoord* ObjLoader::getCoords() const {
    return Coords.data();
}

const unsigned int* ObjLoader::getIndices() const {
    return Indices.data();
}

size_t ObjLoader::numVertices() const {
    return Vertices.size();
}

size_t ObjLoader::numNormals() const {
    return Normals.size();
}

size_t ObjLoader::numCoords() const {
    return Coords.size();
}

size_t ObjLoader::numIndices() const {
    return Indices.size();
}
