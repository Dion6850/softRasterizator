/**
 * @file ModelLoader.h
 * @author zhywyt (zhywyt@yeah.net)
 * @brief 3D Model Loader
 * @version 0.1
 * @date 2025-07-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#include <string>
#include <vector>


struct Vertex {
    float x, y, z;
};

struct Triangle {
    Vertex v0, v1, v2;
};

class ModelLoader {
public:
    ModelLoader() = default;
    ~ModelLoader() = default;

    /**
     * @brief load 3D model from file
     *
     * @param filename object file name
     * @details This function can load only "v" and "f" lines from OBJ files.
     * @return true if successful, false otherwise
     */
    bool loadModel(const std::string& filename);

    const std::vector<Vertex>& getVertices() const;

    const std::vector<Triangle>& getTriangles() const;

private:
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
};
