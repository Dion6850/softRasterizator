/**
 * @file ModelLoader.h
 * @author zhywyt (zhywyt@yeah.net)
 * @brief Enhanced 3D Model Loader for Standard OBJ Files
 * @version 0.2
 * @date 2025-07-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#include <string>
#include <vector>
#include <map>

/**
 * @brief Represents a 3D vertex position
 */
struct Vertex {
    float x, y, z;
};

/**
 * @brief Represents a 2D texture coordinate
 */
struct TextureCoord {
    float u, v;
};

/**
 * @brief Represents a 3D vertex normal
 */
struct Normal {
    float x, y, z;
};

/**
 * @brief Material information from MTL files
 */
struct Material {
    std::string name;                     ///< Material name
    float ambient[3] = {0.2f, 0.2f, 0.2f};    ///< Ambient color (Ka)
    float diffuse[3] = {0.8f, 0.8f, 0.8f};    ///< Diffuse color (Kd)
    float specular[3] = {0.0f, 0.0f, 0.0f};   ///< Specular color (Ks)
    float shininess = 0.0f;               ///< Specular exponent (Ns)
    std::string diffuseTexture;           ///< Diffuse texture map (map_Kd)
    
    /**
     * @brief Check if this material has a diffuse texture
     * @return true if diffuse texture filename is not empty
     */
    bool hasDiffuseTexture() const {
        return !diffuseTexture.empty();
    }
    
    /**
     * @brief Get the full texture path (can be used with base path)
     * @param basePath Base directory path
     * @return Full path to the texture file
     */
    std::string getFullTexturePath(const std::string& basePath) const {
        if (diffuseTexture.empty()) {
            return "";
        }
        if (basePath.empty()) {
            return diffuseTexture;
        }
        return basePath + "/" + diffuseTexture;
    }
};

/**
 * @brief Represents a triangle with vertex, texture, and normal indices
 * @details Each triangle stores indices into the respective arrays for maximum flexibility
 */
struct Triangle {
    Vertex v0, v1, v2;                    ///< Vertex positions
    TextureCoord t0, t1, t2;             ///< Texture coordinates (if available)
    Normal n0, n1, n2;                   ///< Vertex normals (if available)
    bool hasTextures;                     ///< Whether texture coordinates are available
    bool hasNormals;                      ///< Whether vertex normals are available
    std::string materialName;             ///< Name of the material used by this triangle
    const Material* material;             ///< Pointer to the material (set after materials are loaded)
    
    /**
     * @brief Constructor
     */
    Triangle() : hasTextures(false), hasNormals(false), material(nullptr) {}
    
    /**
     * @brief Check if this triangle has a valid material assigned
     * @return true if material is not null
     */
    bool hasMaterial() const {
        return material != nullptr;
    }
    
    /**
     * @brief Check if this triangle has a diffuse texture
     * @return true if material exists and has a diffuse texture
     */
    bool hasDiffuseTexture() const {
        return material != nullptr && !material->diffuseTexture.empty();
    }
    
    /**
     * @brief Get the diffuse texture filename
     * @return Empty string if no texture, otherwise the texture filename
     */
    const std::string& getDiffuseTexture() const {
        static const std::string empty;
        return (material != nullptr) ? material->diffuseTexture : empty;
    }
};

/**
 * @brief Enhanced 3D Model Loader supporting standard OBJ file features
 * @details This class provides comprehensive support for loading standard OBJ files including:
 * - Vertex positions (v)
 * - Texture coordinates (vt)
 * - Vertex normals (vn)
 * - Face definitions with various formats (f)
 * - Material library references (mtllib)
 * - Material usage (usemtl)
 * - Object groups (g, o)
 * - Comments (#)
 */
class ModelLoader {
public:
    /**
     * @brief Default constructor
     */
    ModelLoader() = default;
    
    /**
     * @brief Default destructor
     */
    ~ModelLoader() = default;

    /**
     * @brief Load 3D model from OBJ file
     * @param filename Path to the OBJ file
     * @details This function supports standard OBJ file format including:
     * - Vertex positions (v x y z)
     * - Texture coordinates (vt u v)
     * - Vertex normals (vn x y z)
     * - Face definitions (f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3)
     * - Material libraries (mtllib filename.mtl)
     * - Material usage (usemtl material_name)
     * - Object names (o object_name)
     * - Groups (g group_name)
     * - Comments (# comment text)
     * @return true if successful, false otherwise
     * @retval true Successfully loaded the model
     * @retval false Failed to load the model (file not found, parse error, etc.)
     */
    bool loadModel(const std::string& filename);

    /**
     * @brief Get all vertex positions
     * @return Constant reference to the vector of vertices
     */
    const std::vector<Vertex>& getVertices() const;

    /**
     * @brief Get all triangles
     * @return Constant reference to the vector of triangles
     */
    const std::vector<Triangle>& getTriangles() const;

    /**
     * @brief Get all texture coordinates
     * @return Constant reference to the vector of texture coordinates
     */
    const std::vector<TextureCoord>& getTextureCoords() const;

    /**
     * @brief Get all vertex normals
     * @return Constant reference to the vector of vertex normals
     */
    const std::vector<Normal>& getNormals() const;

    /**
     * @brief Get all materials
     * @return Constant reference to the map of materials (name -> material)
     */
    const std::map<std::string, Material>& getMaterials() const;

    /**
     * @brief Get the current object name
     * @return Current object name
     */
    const std::string& getCurrentObjectName() const;

    /**
     * @brief Get the base path used for resolving relative file paths
     * @return Base path string
     */
    const std::string& getBasePath() const;

    /**
     * @brief Get loading statistics
     * @param vertexCount Output parameter for vertex count
     * @param triangleCount Output parameter for triangle count
     * @param textureCount Output parameter for texture coordinate count
     * @param normalCount Output parameter for normal count
     * @param materialCount Output parameter for material count
     */
    void getStatistics(size_t& vertexCount, size_t& triangleCount, 
                      size_t& textureCount, size_t& normalCount, 
                      size_t& materialCount) const;

private:
    std::vector<Vertex> vertices;              ///< Vertex positions
    std::vector<TextureCoord> textureCoords;   ///< Texture coordinates
    std::vector<Normal> normals;               ///< Vertex normals
    std::vector<Triangle> triangles;           ///< Triangulated faces
    std::map<std::string, Material> materials; ///< Materials by name
    std::string currentObjectName;             ///< Current object name
    std::string currentMaterial;               ///< Current material name
    std::string basePath;                      ///< Base path for resolving relative file paths

    /**
     * @brief Parse a single line from the OBJ file
     * @param line The line to parse
     * @return true if parsing was successful
     */
    bool parseLine(const std::string& line);

    /**
     * @brief Parse vertex position line (v x y z)
     * @param tokens Tokenized line components
     * @return true if parsing was successful
     */
    bool parseVertex(const std::vector<std::string>& tokens);

    /**
     * @brief Parse texture coordinate line (vt u v)
     * @param tokens Tokenized line components
     * @return true if parsing was successful
     */
    bool parseTextureCoord(const std::vector<std::string>& tokens);

    /**
     * @brief Parse vertex normal line (vn x y z)
     * @param tokens Tokenized line components
     * @return true if parsing was successful
     */
    bool parseNormal(const std::vector<std::string>& tokens);

    /**
     * @brief Parse face line (f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3)
     * @param tokens Tokenized line components
     * @return true if parsing was successful
     */
    bool parseFace(const std::vector<std::string>& tokens);

    /**
     * @brief Parse material library line (mtllib filename.mtl)
     * @param tokens Tokenized line components
     * @return true if parsing was successful
     */
    bool parseMaterialLibrary(const std::vector<std::string>& tokens);

    /**
     * @brief Parse material usage line (usemtl material_name)
     * @param tokens Tokenized line components
     * @return true if parsing was successful
     */
    bool parseUseMaterial(const std::vector<std::string>& tokens);

    /**
     * @brief Parse object name line (o object_name)
     * @param tokens Tokenized line components
     * @return true if parsing was successful
     */
    bool parseObjectName(const std::vector<std::string>& tokens);

    /**
     * @brief Split a string by whitespace
     * @param str Input string
     * @return Vector of tokens
     */
    std::vector<std::string> tokenize(const std::string& str) const;

    /**
     * @brief Parse face vertex specification (v/vt/vn format)
     * @param vertexSpec Face vertex specification string
     * @param vertexIndex Output vertex index (1-based, 0 if not specified)
     * @param textureIndex Output texture index (1-based, 0 if not specified)
     * @param normalIndex Output normal index (1-based, 0 if not specified)
     * @return true if parsing was successful
     */
    bool parseFaceVertex(const std::string& vertexSpec, int& vertexIndex, 
                        int& textureIndex, int& normalIndex) const;

    /**
     * @brief Load materials from MTL file
     * @param filename Path to the MTL file
     * @param basePath Base path for resolving relative paths
     * @return true if loading was successful
     */
    bool loadMaterialFile(const std::string& filename, const std::string& basePath);

    /**
     * @brief Parse a single line from MTL file
     * @param line The line to parse
     * @param currentMaterial Reference to current material being parsed
     * @return true if parsing was successful
     */
    bool parseMaterialLine(const std::string& line, Material& currentMaterial);

    /**
     * @brief Update material pointers in all triangles after materials are loaded
     * @details This function should be called after all materials are loaded to set
     * the material pointers in triangles based on their material names
     */
    void updateTriangleMaterialPointers();
};
