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
// xyz is f0, w is f90
static const Eigen::Vector4f f0(0.04f, 0.04f, 0.04f, 1.0f);
static const lsr3d::Color baseColor(1.0f, 1.0f, 1.0f, 1.0f);
static const float roughness = 1.0f;
static const float metallic = 0.0f;

namespace lsr3d
{
    inline Eigen::Vector3f FSchlick(Eigen::Vector3f F0, float NoH) {
        return lsr3d::mix(Eigen::Vector3f(1.0f, 1.0f, 1.0f), F0, lsr3d::pow5(1.0f - NoH));
    }
    /**
     * @brief Normal Distribution Function (NDF) using GGX/Trowbridge-Reitz model
     * @details D = alpha^2 / (pi * ((NoH^2 * (alpha^2 - 1) + 1)^2))
     * @param NoH 
     * @param alpha is roughness
     * @return float 
     */
    inline float DGGX(float NoH, float alpha) {
        float alpha2 = alpha * alpha;
        float denom = (NoH * NoH) * (alpha2 - 1.0f) + 1.0f;
        return alpha2 / (3.14f * denom * denom);
    }
    /**
     * @brief Geometry Function using Schlick-GGX approximation
     * @details G1 = cosTheta / (cosTheta * (1 - k) + k)
     * @param cosTheta 
     * @param k 
     * @return float 
     */
    inline float SchlickGGX(float cosTheta, float k) {
        return cosTheta / (cosTheta * (1.0f - k) + k);
    }
    /**
     * @brief Smith's method for combined geometry term
     * @details G = G1(NoV) * G1(NoL)
     * @param NoV 
     * @param NoL 
     * @param k 
     * @return float 
     */
    inline float GSmith(float NoV, float NoL, float k) {
        return SchlickGGX(NoV, k) * SchlickGGX(NoL, k);
    }
    inline float KDirect(float alpha) {
        return (alpha + 1.0f) * (alpha + 1.0f) / 8.0f;
    }
    lsr3d::Color CalDirectLight(const Eigen::Vector3f& N, const Eigen::Vector3f& L, const Eigen::Vector3f& V, const Color& lc) {
        // finalColor = (diffuse + FDG/(4*NoL*NoV)) * lc * NoL
        lsr3d::Color FinalColor(0.0f, 0.0f, 0.0f, 1.0f);
        Eigen::Vector3f H = (L + V).normalized();
        float NoL = std::max(N.dot(L), 0.0001f);
        float NoV = std::max(N.dot(V), 0.0001f);
        float NoH = std::max(N.dot(H), 0.0001f);
        Eigen::Vector3f F0 = lsr3d::mix(static_cast<Eigen::Vector3f>(f0.head<3>()), baseColor.rgb(), metallic);
        // F
        const Eigen::Vector3f F = FSchlick(F0, NoH);
        // Kd
        const Eigen::Vector3f Kd = (Eigen::Vector3f(1.0f, 1.0f, 1.0f) - F) * (1.0f - metallic);
        lsr3d::Color diffuse(baseColor.rgb().cwiseProduct(Kd / 3.14f), baseColor.a());
        diffuse = diffuse * lc * NoL;
        //D G
        float D, G;
        // set input roughness to roughness^2 to match better with other tools
        float alpha = roughness * roughness;
        D = DGGX(NoH, alpha);
        G = GSmith(NoV, NoL, KDirect(alpha));
        FinalColor = diffuse + lsr3d::Color((F * (D * G / (4.0f * NoL * NoV))), 1.0f);
        FinalColor *= lc * NoL;
        return FinalColor;
    }
}
// lsr3d::Vertex operator*(const Eigen::Matrix4f& mat, const lsr3d::Vertex& v){
//     Eigen::Vector4f res = mat * Eigen::Vector4f(v.position[0], v.position[1], v.position[2], 1.0f);
//     return lsr3d::Vertex(res.head<3>()/res[3]);
// }
/* vertex shader */
void lsr3d::triangleVertexShader::shading(const vertexInputData& input, vertexOutputData& output){
    lsr3d::Vertex v0, v1, v2;
    v0 = input.M * input.triangle.v0;
    v1 = input.M * input.triangle.v1;
    v2 = input.M * input.triangle.v2;

    lsr3d::Vertex mvp_v0, mvp_v1, mvp_v2;
    mvp_v0 = input.VP * v0;
    mvp_v1 = input.VP * v1;
    mvp_v2 = input.VP * v2;

    // 获取投影二维坐标
    Eigen::Vector2f v0_screen = ((mvp_v0.xy() + Eigen::Vector2f::Ones()) * 0.5f).cwiseProduct(Eigen::Vector2f(input.width, input.height));
    Eigen::Vector2f v1_screen = ((mvp_v1.xy() + Eigen::Vector2f::Ones()) * 0.5f).cwiseProduct(Eigen::Vector2f(input.width, input.height));
    Eigen::Vector2f v2_screen = ((mvp_v2.xy() + Eigen::Vector2f::Ones()) * 0.5f).cwiseProduct(Eigen::Vector2f(input.width, input.height));

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

    if(input.triangle.hasNormals == false){
        // 计算面法线
        lsr3d::Normal faceNormal = input.M * input.triangle.n;
        output.triangle.v0.normal = faceNormal;
        output.triangle.v1.normal = faceNormal;
        output.triangle.v2.normal = faceNormal;
        output.triangle.hasNormals = true;
    }
    else {
        output.triangle.v0.normal = input.M * input.triangle.v0.normal;
        output.triangle.v1.normal = input.M * input.triangle.v1.normal;
        output.triangle.v2.normal = input.M * input.triangle.v2.normal;
    }


    output.triangle.t0 = input.triangle.t0;
    output.triangle.t1 = input.triangle.t1;
    output.triangle.t2 = input.triangle.t2;

    output.triangle.materialName = input.triangle.materialName;
    output.triangle.material = input.triangle.material;
    output.triangle.hasTextures = input.triangle.hasTextures;
    output.triangle.hasNormals = input.triangle.hasNormals;

    output.uniform = input.uniform;
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
    Eigen::Vector3f V = (input.uniform->viewPos - position).normalized();
    /* TODO : cal lighting*/
    // dir light
    for (const auto& light : *(input.uniform->dirLights)) {
        Eigen::Vector3f L = -light.second.direction.normalized();
        lightingColor += lsr3d::CalDirectLight(n0, L, V, light.second.color) * light.second.intensity;
    }
    // spot light
    for (const auto& light : *(input.uniform->spotLights)) {
        // 计算光线方向
        Eigen::Vector3f L = (light.second.position - position).normalized();
        // 计算光线与聚光灯的角度
        float cos = std::cos(light.second.cutoffAngle);
        if (L.dot(light.second.direction.normalized()) < cos) {
            continue;
        }
        lightingColor += lsr3d::CalDirectLight(n0, L, V, light.second.color) * light.second.intensity;
    }
    // point light
    for (const auto& light : *(input.uniform->pointLights)) {
        Eigen::Vector3f L = (light.second.position).normalized();
        lightingColor += lsr3d::CalDirectLight(n0, L, V, light.second.color) * light.second.intensity;
    }
    if (material.isValid()) {
        // 使用材质进行环境光照
        if (const auto& image = input.images->find(material.imageHandle);
            image != input.images->end()) {
                // 使用纹理进行环境光照
                materialColor = image->second.SampleNearest(uv0.uv);
        }
    }
    else {
        // 默认颜色，uv插值
        materialColor.color = lsr3d::Color(0.0f, 0.0f, 0.0f, 1.0f); // 默认黑色
    }
    output.color = lightingColor + materialColor;
}