#include <ModelLoader.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

bool ModelLoader::loadModel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Clear previous data
    vertices.clear();
    textureCoords.clear();
    normals.clear();
    triangles.clear();
    materials.clear();
    currentObjectName.clear();
    currentMaterial.clear();
    
    std::string line;
    while (std::getline(file, line)) {
        if (!parseLine(line)) {
            // Continue parsing even if individual lines fail
            // This provides better error tolerance
        }
    }
    file.close();
    
    return !vertices.empty() && !triangles.empty();
}

bool ModelLoader::parseLine(const std::string& line) {
    // Skip empty lines and comments
    if (line.empty() || line[0] == '#') {
        return true;
    }
    
    auto tokens = tokenize(line);
    if (tokens.empty()) {
        return true;
    }
    
    const std::string& prefix = tokens[0];
    
    if (prefix == "v") {
        return parseVertex(tokens);
    } else if (prefix == "vt") {
        return parseTextureCoord(tokens);
    } else if (prefix == "vn") {
        return parseNormal(tokens);
    } else if (prefix == "f") {
        return parseFace(tokens);
    } else if (prefix == "mtllib") {
        return parseMaterialLibrary(tokens);
    } else if (prefix == "usemtl") {
        return parseUseMaterial(tokens);
    } else if (prefix == "o") {
        return parseObjectName(tokens);
    } else if (prefix == "g") {
        // Group names - currently ignored but can be extended
        return true;
    } else if (prefix == "s") {
        // Smoothing groups - currently ignored but can be extended
        return true;
    }
    
    // Unknown line type - ignore but don't fail
    return true;
}

bool ModelLoader::parseVertex(const std::vector<std::string>& tokens) {
    if (tokens.size() < 4) {
        return false;
    }
    
    try {
        float x = std::stof(tokens[1]);
        float y = std::stof(tokens[2]);
        float z = std::stof(tokens[3]);
        vertices.push_back({x, y, z});
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool ModelLoader::parseTextureCoord(const std::vector<std::string>& tokens) {
    if (tokens.size() < 3) {
        return false;
    }
    
    try {
        float u = std::stof(tokens[1]);
        float v = std::stof(tokens[2]);
        textureCoords.push_back({u, v});
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool ModelLoader::parseNormal(const std::vector<std::string>& tokens) {
    if (tokens.size() < 4) {
        return false;
    }
    
    try {
        float x = std::stof(tokens[1]);
        float y = std::stof(tokens[2]);
        float z = std::stof(tokens[3]);
        normals.push_back({x, y, z});
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool ModelLoader::parseFace(const std::vector<std::string>& tokens) {
    if (tokens.size() < 4) {
        return false;
    }
    
    std::vector<int> vertexIndices;
    std::vector<int> textureIndices;
    std::vector<int> normalIndices;
    
    // Parse each vertex specification
    for (size_t i = 1; i < tokens.size(); ++i) {
        int vIdx = 0, tIdx = 0, nIdx = 0;
        if (!parseFaceVertex(tokens[i], vIdx, tIdx, nIdx)) {
            return false;
        }
        
        vertexIndices.push_back(vIdx);
        textureIndices.push_back(tIdx);
        normalIndices.push_back(nIdx);
    }
    
    // Triangulate the face (fan triangulation)
    for (size_t i = 1; i + 1 < vertexIndices.size(); ++i) {
        Triangle triangle;
        
        // Get vertex positions (convert 1-based to 0-based indexing)
        int v0 = vertexIndices[0] - 1;
        int v1 = vertexIndices[i] - 1;
        int v2 = vertexIndices[i + 1] - 1;
        
        if (v0 < 0 || v0 >= static_cast<int>(vertices.size()) ||
            v1 < 0 || v1 >= static_cast<int>(vertices.size()) ||
            v2 < 0 || v2 >= static_cast<int>(vertices.size())) {
            return false;
        }
        
        triangle.v0 = vertices[v0];
        triangle.v1 = vertices[v1];
        triangle.v2 = vertices[v2];
        
        // Handle texture coordinates if available
        triangle.hasTextures = false;
        if (textureIndices[0] > 0 && textureIndices[i] > 0 && textureIndices[i + 1] > 0) {
            int t0 = textureIndices[0] - 1;
            int t1 = textureIndices[i] - 1;
            int t2 = textureIndices[i + 1] - 1;
            
            if (t0 >= 0 && t0 < static_cast<int>(textureCoords.size()) &&
                t1 >= 0 && t1 < static_cast<int>(textureCoords.size()) &&
                t2 >= 0 && t2 < static_cast<int>(textureCoords.size())) {
                triangle.t0 = textureCoords[t0];
                triangle.t1 = textureCoords[t1];
                triangle.t2 = textureCoords[t2];
                triangle.hasTextures = true;
            }
        }
        
        // Handle normals if available
        triangle.hasNormals = false;
        if (normalIndices[0] > 0 && normalIndices[i] > 0 && normalIndices[i + 1] > 0) {
            int n0 = normalIndices[0] - 1;
            int n1 = normalIndices[i] - 1;
            int n2 = normalIndices[i + 1] - 1;
            
            if (n0 >= 0 && n0 < static_cast<int>(normals.size()) &&
                n1 >= 0 && n1 < static_cast<int>(normals.size()) &&
                n2 >= 0 && n2 < static_cast<int>(normals.size())) {
                triangle.n0 = normals[n0];
                triangle.n1 = normals[n1];
                triangle.n2 = normals[n2];
                triangle.hasNormals = true;
            }
        }
        
        triangles.push_back(triangle);
    }
    
    return true;
}

bool ModelLoader::parseMaterialLibrary(const std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        return false;
    }
    
    // TODO: Implement MTL file loading
    // For now, just acknowledge the material library reference
    return true;
}

bool ModelLoader::parseUseMaterial(const std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        return false;
    }
    
    currentMaterial = tokens[1];
    return true;
}

bool ModelLoader::parseObjectName(const std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        return false;
    }
    
    currentObjectName = tokens[1];
    return true;
}

std::vector<std::string> ModelLoader::tokenize(const std::string& str) const {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

bool ModelLoader::parseFaceVertex(const std::string& vertexSpec, int& vertexIndex, 
                                 int& textureIndex, int& normalIndex) const {
    vertexIndex = textureIndex = normalIndex = 0;
    
    // Split by '/' character
    std::vector<std::string> parts;
    std::string current;
    
    for (char c : vertexSpec) {
        if (c == '/') {
            parts.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    parts.push_back(current);
    
    if (parts.empty()) {
        return false;
    }
    
    try {
        // Parse vertex index (required)
        if (!parts[0].empty()) {
            vertexIndex = std::stoi(parts[0]);
        } else {
            return false;
        }
        
        // Parse texture index (optional)
        if (parts.size() > 1 && !parts[1].empty()) {
            textureIndex = std::stoi(parts[1]);
        }
        
        // Parse normal index (optional)
        if (parts.size() > 2 && !parts[2].empty()) {
            normalIndex = std::stoi(parts[2]);
        }
        
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

const std::vector<Vertex>& ModelLoader::getVertices() const {
    return vertices;
}

const std::vector<Triangle>& ModelLoader::getTriangles() const {
    return triangles;
}

const std::vector<TextureCoord>& ModelLoader::getTextureCoords() const {
    return textureCoords;
}

const std::vector<Normal>& ModelLoader::getNormals() const {
    return normals;
}

const std::map<std::string, Material>& ModelLoader::getMaterials() const {
    return materials;
}

const std::string& ModelLoader::getCurrentObjectName() const {
    return currentObjectName;
}

void ModelLoader::getStatistics(size_t& vertexCount, size_t& triangleCount, 
                               size_t& textureCount, size_t& normalCount, 
                               size_t& materialCount) const {
    vertexCount = vertices.size();
    triangleCount = triangles.size();
    textureCount = textureCoords.size();
    normalCount = normals.size();
    materialCount = materials.size();
}
