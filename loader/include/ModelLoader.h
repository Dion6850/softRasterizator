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

    // 加载模型文件，返回是否成功
    bool loadModel(const std::string& filename);

    // 获取顶点数据
    const std::vector<Vertex>& getVertices() const;

    // 获取三角形数据
    const std::vector<Triangle>& getTriangles() const;

private:
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
};
