/**
 * @file shader.h
 * @author dion (hduer_zdy@outlook.com)
 * @brief
 * @version 0.1
 * @date 2025/7/15
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <Lsr3D/core/shader.h>


/* vertex shader */
void lsr3d::triangleVertexShader::shading(const vertexInputData& input, vertexOutputData& output){
    lsr3d::Vertex v0, v1, v2;
    v0.position = input.MVP * input.triangle.v0.position;
    v1.position = input.MVP * input.triangle.v1.position;
    v2.position = input.MVP * input.triangle.v2.position;


    v0.position /= v0.position.w();
    v1.position /= v1.position.w();
    v2.position /= v2.position.w();

    // 转换到屏幕坐标系 (假设视口大小为1024x1024)
    v0.position.x() = (v0.position.x() + 1.0f) * 0.5f * input.width;
    v0.position.y() = (-v0.position.y() + 1.0f) * 0.5f * input.height;
    v1.position.x() = (v1.position.x() + 1.0f) * 0.5f * input.width;
    v1.position.y() = (-v1.position.y() + 1.0f) * 0.5f * input.height;
    v2.position.x() = (v2.position.x() + 1.0f) * 0.5f * input.width;
    v2.position.y() = (-v2.position.y() + 1.0f) * 0.5f * input.height;

    // 获取投影二维坐标
    Eigen::Vector2f v0_screen = v0.position.head<2>();
    Eigen::Vector2f v1_screen = v1.position.head<2>();
    Eigen::Vector2f v2_screen = v2.position.head<2>();

    // 面剔除
    float area = cross2F(v1_screen - v0_screen, v2_screen - v0_screen);
    if (isEnabledFaceCull) {
        if (area >= 0) {
            // Backface culling: discard this triangle
            output.discard = true;
            return;
        }
    }
    output.triangle.v0.position = v0.position;
    output.triangle.v1.position = v1.position;
    output.triangle.v2.position = v2.position;

    output.triangle.c0 = input.triangle.c0;
    output.triangle.c1 = input.triangle.c1;
    output.triangle.c2 = input.triangle.c2;

    output.triangle.n0 = input.triangle.n0;
    output.triangle.n1 = input.triangle.n1;
    output.triangle.n2 = input.triangle.n2;

    output.triangle.materialName = input.triangle.materialName;
    output.triangle.material_ = input.triangle.material_;
    output.triangle.hasTextures = input.triangle.hasTextures;
    output.triangle.hasNormals = input.triangle.hasNormals;
}



/* fragment shader */
void lsr3d::triangleFragmentShader::shading(const fragementInputData& input, fragementOutputData& output){
    // 获取顶点数据
    const lsr3d::Uv& v0 = input.position;
    const lsr3d::TextureCoord& uv0 = input.textureCoord;
    const lsr3d::Normal& n0 = input.normal;                 // to cal lighting
    const lsr3d::Material* material_ = input.material_;
    if (material_ != nullptr) {
        // 使用材质进行环境光照
        if (const auto& image_ = input.images.find(material_->imageHandle);
            image_ != input.images.end()) {
                // 使用纹理进行环境光照
                output.color = image_->second.SampleNearest(uv0.uv);
        }
        /* TODO : cal lighting*/
    }
    else {
        // 默认颜色，uv插值
        output.color = input.color;
    }
}