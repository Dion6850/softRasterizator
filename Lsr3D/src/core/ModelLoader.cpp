#include <Lsr3D/core/ModelLoader.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <filesystem>

bool ModelLoader::loadModel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Extract base path from the filename
    std::filesystem::path filePath(filename);
    basePath = filePath.parent_path().string();
    
    // Clear previous data
    {
        vertices.clear();
        textureCoords.clear();
        normals.clear();
        triangles.clear();
        images.clear();
        materials.clear();
        materialNameToHandle.clear();

        currentObjectName.clear();
        currentMaterial.clear();

        currentVertexIndex = 0;
        currentTextureIndex = 0;
        currentNormalIndex = 0;
        currentTriangleIndex = 0;
        currentImageIndex = 0;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!parseLine(line)) {
            // Continue parsing even if individual lines fail
            // This provides better error tolerance
        }
    }
    file.close();
    
    // Update material pointers in triangles after all materials are loaded
    updateTriangleMaterialPointers();
    
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
        vertices.emplace(lsr3d::VertexHandle(currentVertexIndex++), lsr3d::Vertex(x, y, z));
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
        textureCoords.emplace(lsr3d::TextureCoordHandle(currentTextureIndex++), lsr3d::TextureCoord(u, v));
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
        normals.emplace(lsr3d::NormalHandle(currentNormalIndex++), lsr3d::Normal(x, y, z));
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
        
        /* use handle without check */
        // if (v0 < 0 || v0 >= static_cast<int>(vertices.size()) ||
        //     v1 < 0 || v1 >= static_cast<int>(vertices.size()) ||
        //     v2 < 0 || v2 >= static_cast<int>(vertices.size())) {
        //     return false;
        // }
        
        triangle.v0 = lsr3d::VertexHandle(v0);
        triangle.v1 = lsr3d::VertexHandle(v1);
        triangle.v2 = lsr3d::VertexHandle(v2);
        
        // Handle texture coordinates if available
        triangle.hasTextures = false;
        if (textureIndices[0] > 0 && textureIndices[i] > 0 && textureIndices[i + 1] > 0) {
            int t0 = textureIndices[0] - 1;
            int t1 = textureIndices[i] - 1;
            int t2 = textureIndices[i + 1] - 1;
            
            /* use handle without check */
            // if (t0 >= 0 && t0 < static_cast<int>(textureCoords.size()) &&
            //     t1 >= 0 && t1 < static_cast<int>(textureCoords.size()) &&
            //     t2 >= 0 && t2 < static_cast<int>(textureCoords.size())) {
               triangle.t0 = lsr3d::TextureCoordHandle(t0);
               triangle.t1 = lsr3d::TextureCoordHandle(t1);
               triangle.t2 = lsr3d::TextureCoordHandle(t2);
               triangle.hasTextures = true;
            // }
        }
        
        // Handle normals if available
        triangle.hasNormals = false;
        if (normalIndices[0] > 0 && normalIndices[i] > 0 && normalIndices[i + 1] > 0) {
            int n0 = normalIndices[0] - 1;
            int n1 = normalIndices[i] - 1;
            int n2 = normalIndices[i + 1] - 1;
            
            // if (n0 >= 0 && n0 < static_cast<int>(normals.size()) &&
            //     n1 >= 0 && n1 < static_cast<int>(normals.size()) &&
            //     n2 >= 0 && n2 < static_cast<int>(normals.size())) {
                triangle.n0 = lsr3d::NormalHandle(n0);
                triangle.n1 = lsr3d::NormalHandle(n1);
                triangle.n2 = lsr3d::NormalHandle(n2);
            //     triangle.hasNormals = true;
            // }
        }
        
        // Set the current material name for this triangle
        triangle.materialName = currentMaterial;
        
        triangles.emplace(lsr3d::TriangleHandle(currentTriangleIndex++), triangle);
    }
    
    return true;
}

bool ModelLoader::parseMaterialLibrary(const std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        return false;
    }
    
    // Get the MTL filename
    std::string mtlFilename = tokens[1];
    
    return loadMaterialFile(mtlFilename, basePath);
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

const VertexDatas& ModelLoader::getVertices() const {
    return vertices;
}

const TriangleDatas& ModelLoader::getTriangles() const {
    return triangles;
}

const TextureCoordDatas& ModelLoader::getTextureCoords() const {
    return textureCoords;
}

const NormalDatas& ModelLoader::getNormals() const {
    return normals;
}

const ImageDatas& ModelLoader::getImages() const {
    return images;
}

const MaterialDatas& ModelLoader::getMaterials() const {
    return materials;
}

const std::string& ModelLoader::getCurrentObjectName() const {
    return currentObjectName;
}

const std::string& ModelLoader::getBasePath() const {
    return basePath;
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

bool ModelLoader::loadMaterialFile(const std::string& filename, const std::string& basePath) {
    std::filesystem::path fullPath;
    
    if (basePath.empty()) {
        fullPath = filename;
    } else {
        fullPath = std::filesystem::path(basePath) / filename;
    }
    
    std::ifstream file(fullPath);
    if (!file.is_open()) {
        // Try alternative paths or report error
        std::cerr << "Warning: Cannot open material file: " << fullPath << std::endl;
        return false;
    }
    
    Material currentMaterial;
    std::string line;
    
    while (std::getline(file, line)) {
        if (!parseMaterialLine(line, currentMaterial)) {
            // Continue parsing even if individual lines fail
            std::cerr << "Warning: Failed to parse material line: " << line << std::endl;
            // not throw or exit
        }
    }
    // Add the last material if it has a name
    if (!currentMaterial.name.empty()) {
        MaterialHandle handle = MaterialHandle(currentMaterialIndex++);
        materials[handle] = currentMaterial;
        materialNameToHandle[currentMaterial.name] = handle;
    }
    
    file.close();
    return true;
}

bool ModelLoader::parseMaterialLine(const std::string& line, Material& currentMaterial) {
    // Skip empty lines and comments
    if (line.empty() || line[0] == '#') {
        return true;
    }
    
    auto tokens = tokenize(line);
    if (tokens.empty()) {
        return true;
    }
    
    const std::string& prefix = tokens[0];
    
    if (prefix == "newmtl") {
        // Start new material
        if (tokens.size() < 2) {
            return false;
        }
        
        // Save previous material if it exists
        if (!currentMaterial.name.empty()) {
            MaterialHandle handle = MaterialHandle(currentMaterialIndex++);
            materials[handle] = currentMaterial;
            materialNameToHandle[currentMaterial.name] = handle;
        }
        
        // Initialize new material
        currentMaterial = Material();
        currentMaterial.name = tokens[1];
        return true;
    }
    
    // Skip if no current material
    if (currentMaterial.name.empty()) {
        return true;
    }
    
    if (prefix == "Ka") {
        // Ambient color
        if (tokens.size() >= 4) {
            try {
                currentMaterial.ambient[0] = std::stof(tokens[1]);
                currentMaterial.ambient[1] = std::stof(tokens[2]);
                currentMaterial.ambient[2] = std::stof(tokens[3]);
                return true;
            } catch (const std::exception&) {
                return false;
            }
        }
    } else if (prefix == "Kd") {
        // Diffuse color
        if (tokens.size() >= 4) {
            try {
                currentMaterial.diffuse[0] = std::stof(tokens[1]);
                currentMaterial.diffuse[1] = std::stof(tokens[2]);
                currentMaterial.diffuse[2] = std::stof(tokens[3]);
                return true;
            } catch (const std::exception&) {
                return false;
            }
        }
    } else if (prefix == "Ks") {
        // Specular color
        if (tokens.size() >= 4) {
            try {
                currentMaterial.specular[0] = std::stof(tokens[1]);
                currentMaterial.specular[1] = std::stof(tokens[2]);
                currentMaterial.specular[2] = std::stof(tokens[3]);
                return true;
            } catch (const std::exception&) {
                return false;
            }
        }
    } else if (prefix == "Ns") {
        // Specular exponent (shininess)
        if (tokens.size() >= 2) {
            try {
                currentMaterial.shininess = std::stof(tokens[1]);
                return true;
            } catch (const std::exception&) {
                return false;
            }
        }
    } else if (prefix == "map_Kd") {
        // Diffuse texture map
        if (tokens.size() >= 2) {
            currentMaterial.textureName = tokens[1];
            lsr3d::ImageHandle handle = lsr3d::ImageHandle(currentImageIndex++);
            currentMaterial.imageHandle = handle;
            try {
                Image image = lsr3d::loadImage(currentMaterial.getFullTexturePath(basePath));
                images.emplace(handle, image);
            }
            catch (const std::exception& e) {
                std::cerr << "Error loading texture: " << e.what() << std::endl;
                return false;
            }
            return true;
        }
    }
    
    // Unknown or unsupported line - ignore but don't fail
    return true;
}

void ModelLoader::updateTriangleMaterialPointers() {
    for (auto& triangle_ : triangles) {
        auto& triangle = triangle_.second;
        if (!triangle.materialName.empty()) {
            auto it = materialNameToHandle.find(triangle.materialName);
            if (it != materialNameToHandle.end()) {
                triangle.material = it->second;
            } else {
                triangle.material = MaterialHandle();
                std::cerr << "Warning: Material '" << triangle.materialName << "' not found" << std::endl;
            }
        } else {
            // No material assigned
            triangle.material = MaterialHandle();
        }
    }
}
