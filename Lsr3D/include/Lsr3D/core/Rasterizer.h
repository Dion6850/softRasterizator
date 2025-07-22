/**
 * @file Rasterizer.h
 * @author dion (hduer_zdy@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2025/7/13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef RASTERIZER_H
#define RASTERIZER_H
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Lsr3D/core/camera.h>
#include <Lsr3D/utils/MVP.h>
#include <Lsr3D/core/resource.h>
#include <Lsr3D/core/shader.h>
#include <vector>
using lsr3d::cross;
namespace lsr3d {

class Rasterizer {
public:
    Rasterizer(int widht_, int height_, bool isEnableEarlyZBuffer_ = true);
    ~Rasterizer() = default;
    bool setFragmentShader(lsr3d::triangleFragmentShader shader);
    void rasterization(const lsr3d::vertexOutputData& input, const lsr3d::ImageDatas& images);
    void resize(int width_, int height_);
    void clearDepthBuffer();
    void enableEarlyZBuffer(bool enable = true);
    void enableBackFaceCulling(bool enable = true);
    void getViewportSize(int& width, int& height) const;
private:
    bool isEnableEarlyZBuffer = true; ///< 当fragmentshader 支持并开启该开关时，会尝试启用early Z-buffer
    bool isEnableBackFaceCulling = true; ///< 是否启用背面剔除
    // for early-z-buffer
    int width, height;
    std::vector<std::vector<float>> depthBuffer; ///< 深度缓冲区

    lsr3d::triangleFragmentShader fragmentShader; ///< 片段着色器

    lsr3d::TextureCoord interpolateUV(float w0, float w1, float w2, const lsr3d::vertexOutputData& input) const {
        // 线性插值纹理坐标
        return (input.triangle.t0 * w0 + input.triangle.t1 * w1 + input.triangle.t2 * w2);
    }
    lsr3d::Normal interpolateNormal(float w0, float w1, float w2, const lsr3d::vertexOutputData& input) const {
        // 线性插值法线
        return (input.triangle.n0 * w0 + input.triangle.n1 * w1 + input.triangle.n2 * w2).normalized();
    }
    lsr3d::Color interpolateColor(float w0, float w1, float w2, const lsr3d::vertexOutputData& input) const {
        // 线性插值颜色
        return (input.triangle.c0 * w0 + input.triangle.c1 * w1 + input.triangle.c2 * w2);
    }
};

} // Rasterizer

#endif //RASTERIZER_H
