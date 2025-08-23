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

namespace lsr3d
{
    lsr3d::Color CalDirectLight(const Eigen::Vector3f& normal, const Eigen::Vector3f& dir, const Color& color) {
        return color * lsr3d::abs(normal.dot(dir));
    }
}
/* vertex shader */
void lsr3d::triangleVertexShader::shading(const vertexInputData& input, vertexOutputData& output){
    lsr3d::Vertex v0, v1, v2;
    v0.position = input.M * input.triangle.v0.position;
    v1.position = input.M * input.triangle.v1.position;
    v2.position = input.M * input.triangle.v2.position;


    v0.position /= v0.position.w();
    v1.position /= v1.position.w();
    v2.position /= v2.position.w();

    Eigen::Vector4f mvp_v0 = input.VP * v0.position;
    Eigen::Vector4f mvp_v1 = input.VP * v1.position;
    Eigen::Vector4f mvp_v2 = input.VP * v2.position;

    mvp_v0 /= mvp_v0.w();
    mvp_v1 /= mvp_v1.w();
    mvp_v2 /= mvp_v2.w();

    mvp_v0[1] = -mvp_v0[1]; // Flip Y coordinate for OpenGL compatibility
    mvp_v1[1] = -mvp_v1[1];
    mvp_v2[1] = -mvp_v2[1];

    // 获取投影二维坐标
    Eigen::Vector2f v0_screen = ((mvp_v0.head<2>() + Eigen::Vector2f::Ones()) * 0.5f).cwiseProduct(Eigen::Vector2f(input.width, input.height));
    Eigen::Vector2f v1_screen = ((mvp_v1.head<2>() + Eigen::Vector2f::Ones()) * 0.5f).cwiseProduct(Eigen::Vector2f(input.width, input.height));
    Eigen::Vector2f v2_screen = ((mvp_v2.head<2>() + Eigen::Vector2f::Ones()) * 0.5f).cwiseProduct(Eigen::Vector2f(input.width, input.height));
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

    output.triangle.s0 = v0_screen;
    output.triangle.s1 = v1_screen;
    output.triangle.s2 = v2_screen;

    output.triangle.c0 = input.triangle.c0;
    output.triangle.c1 = input.triangle.c1;
    output.triangle.c2 = input.triangle.c2;

    // 生成法线

    output.triangle.n0 = input.triangle.n0;
    output.triangle.n1 = input.triangle.n1;
    output.triangle.n2 = input.triangle.n2;

    output.triangle.t0 = input.triangle.t0;
    output.triangle.t1 = input.triangle.t1;
    output.triangle.t2 = input.triangle.t2;

    output.triangle.materialName = input.triangle.materialName;
    output.triangle.material = input.triangle.material;
    output.triangle.hasTextures = input.triangle.hasTextures;
    output.triangle.hasNormals = input.triangle.hasNormals;
}



/* fragment shader */
void lsr3d::triangleFragmentShader::shading(const fragmentInputData& input, fragementOutputData& output){
    // 获取顶点数据
    const lsr3d::PVec& position = input.position;
    const lsr3d::Uv screenSpacePosition = input.screenSpacePosition;
    const lsr3d::TextureCoord& uv0 = input.textureCoord;
    const lsr3d::Normal& n0 = input.normal;                 // to cal lighting
    const auto material = input.material;
    lsr3d::Color lightingColor;
    lsr3d::Color materialColor;
    /* TODO : cal lighting*/
    // dir light
    for (const auto& light : input.dirLights) {
        lightingColor += lsr3d::CalDirectLight(n0, light.second.direction.head<3>().normalized(), light.second.color) * light.second.intensity;
    }
    // spot light
    for (const auto& light : input.spotLights) {
        // 计算光线方向
        Eigen::Vector3f lightDir = (position - light.second.position).head<3>().normalized();
        // 计算光线与聚光灯的角度
        float cos = std::cos(light.second.cutoffAngle);
        if (lightDir.dot(light.second.direction.head<3>().normalized()) < cos) {
            continue;
        }
        lightingColor += lsr3d::CalDirectLight(n0.normal, lightDir, light.second.color) * light.second.intensity;
    }
    // point light
    for (const auto& light : input.pointLights) {
        lightingColor += lsr3d::CalDirectLight(n0.normal, (position - light.second.position).head<3>().normalized(), light.second.color) * light.second.intensity;
    }
    if (material.isValid()) {
        // 使用材质进行环境光照
        if (const auto& image = input.images.find(material.imageHandle);
            image != input.images.end()) {
                // 使用纹理进行环境光照
                materialColor = image->second.SampleNearest(uv0.uv)/255.0f;
        }
    }
    else {
        // 默认颜色，uv插值
        materialColor.color = lsr3d::Color(0.5f, 0.5f, 0.5f, 1.0f); // 默认灰色
    }
    output.color = lightingColor + materialColor;
}