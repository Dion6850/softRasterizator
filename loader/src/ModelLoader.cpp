#include <ModelLoader.h>
#include <fstream>
#include <sstream>

bool ModelLoader::loadModel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    vertices.clear();
    triangles.clear();
    std::string line;
    std::vector<Vertex> temp_vertices;
    std::vector<std::vector<int>> temp_faces;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            temp_vertices.push_back({x, y, z});
        } else if (prefix == "f") {
            std::vector<int> face_indices;
            std::string vert_str;
            while (iss >> vert_str) {
                size_t pos = vert_str.find('/');
                int idx = std::stoi(pos == std::string::npos ? vert_str : vert_str.substr(0, pos));
                face_indices.push_back(idx - 1); // OBJ indices are 1-based, convert to 0-based
            }
            if (face_indices.size() >= 3) {
                temp_faces.push_back(face_indices);
            }
        }
    }
    file.close();
    vertices = temp_vertices;
    for (const auto& face : temp_faces) {
        for (size_t i = 1; i + 1 < face.size(); ++i) {
            triangles.push_back({
                temp_vertices[face[0]],
                temp_vertices[face[i]],
                temp_vertices[face[i+1]]
            });
        }
    }
    return !vertices.empty() && !triangles.empty();
}

const std::vector<Vertex>& ModelLoader::getVertices() const {
    return vertices;
}

const std::vector<Triangle>& ModelLoader::getTriangles() const {
    return triangles;
}
