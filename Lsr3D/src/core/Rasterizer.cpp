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

#include <Lsr3D/core/Rasterizer.h>
#include <omp.h>
namespace lsr3d {
    Rasterizer::Rasterizer(int width_, int height_, bool isEnableEarlyZBuffer_) : width(width_), height(height_), isEnableEarlyZBuffer(isEnableEarlyZBuffer_) {
        if (isEnableEarlyZBuffer) {
            depthBuffer.resize(height, std::vector<float>(width, std::numeric_limits<float>::max()));
        }
    } ///< Constructor initializes the rasterizer with specified width and height
    void Rasterizer::rasterization(const lsr3d::vertexOutputData& input, const lsr3d::ImageDatas& images,
        const lsr3d::DirectionalLightDatas& dirLights, const lsr3d::PointLightDatas& pointLights, const lsr3d::SpotLightDatas& spotLights) {
        if (input.discard) {
            return; // Skip rasterization if the triangle is discarded
        }
        lsr3d::SVec v0_screen = input.triangle.s0;
        lsr3d::SVec v1_screen = input.triangle.s1;
        lsr3d::SVec v2_screen = input.triangle.s2;
        // bundding box
        int minx = std::min({v0_screen.x(), v1_screen.x(), v2_screen.x()});
        int miny = std::min({v0_screen.y(), v1_screen.y(), v2_screen.y()});
        int maxx = std::max({v0_screen.x(), v1_screen.x(), v2_screen.x()});
        int maxy = std::max({v0_screen.y(), v1_screen.y(), v2_screen.y()});
        // Clamp bounding box to image dimensions
        minx = std::max(minx, 0);
        miny = std::max(miny, 0);
        maxx = std::min(maxx, width - 1);
        maxy = std::min(maxy, height - 1);

        float area = cross2F(v1_screen - v0_screen, v2_screen - v1_screen);
        if(isEnableBackFaceCulling && area >= 0) {
            return; // Backface culling: skip this triangle
        }
        // Rasterize the triangle within the bounding box
        // #pragma omp parallel for schedule(dynamic, 16)
        for (int y = miny; y <= maxy; ++y) {
            for (int x = minx; x <= maxx; ++x) {
            // Perform edge function tests and depth tests here
            lsr3d::SVec p(x + 0.5f, y + 0.5f); // Center of pixel
            float w0 = cross2F(v1_screen - p, v2_screen - p) / area;
            float w1 = cross2F(v2_screen - p, v0_screen - p) / area;
            float w2 = cross2F(v0_screen - p, v1_screen - p) / area;
            if (w0 < 0 || w1 < 0 || w2 < 0) {
                continue; // Pixel is outside the triangle
            }
            float z = w0 * input.triangle.v0.position.z() + w1 * input.triangle.v1.position.z() + w2 * input.triangle.v2.position.z();
            /* early-z test */
            if (isEnableEarlyZBuffer) {
                // Early Z-buffer test (height, width)
                if (depthBuffer[y][x] >= z) {
                    continue; // Skip pixel if depth test fails
                }
            }
            lsr3d::fragmentInputData fragmentInput{
                .position = interpolate(w0, w1, w2, input.triangle.v0.xyz(), input.triangle.v1.xyz(), input.triangle.v2.xyz()),
                .screenSpacePosition = p,
                .textureCoord = interpolate(w0, w1, w2, input.triangle.t0, input.triangle.t1, input.triangle.t2),
                .normal = interpolate(w0, w1, w2, input.triangle.v0.normal, input.triangle.v1.normal, input.triangle.v2.normal).normalized(),
                .color = interpolate(w0, w1, w2, input.triangle.c0,input.triangle.c1,input.triangle.c2),
                .material = input.triangle.material,
                .images = &images,
                .uniform = input.uniform,
            };
            lsr3d::fragementOutputData fragmentOutput;
            fragmentShader.shading(fragmentInput, fragmentOutput);
            /* depth test */
            if (depthBuffer[y][x] >= z) {
                continue; // Skip pixel if depth test fails
            }
            depthBuffer[y][x] = z; // Update depth buffer
            SetPixel(x, y, fragmentOutput.color);
            }
        }
    }
    bool Rasterizer::setFragmentShader(lsr3d::triangleFragmentShader shader) {
        fragmentShader = std::move(shader);
        return true; // Assuming setting the shader always succeeds
    }
    void Rasterizer::resize(int width_, int height_) {
        width = width_;
        height = height_;
        if(isEnableEarlyZBuffer)
            depthBuffer.resize(height, std::vector<float>(width, -std::numeric_limits<float>::max()));
    } ///< Resize the rasterizer and clear the depth buffer
    void Rasterizer::clearDepthBuffer() {
        for (auto& row : depthBuffer) {
            std::fill(row.begin(), row.end(), -std::numeric_limits<float>::max());
        }
    } ///< 清空深度缓冲区
    void Rasterizer::enableEarlyZBuffer(bool enable) {
        isEnableEarlyZBuffer = enable;
        // try to create or resize depth buffer
        if (enable&& (depthBuffer.empty() || depthBuffer.size() != height || depthBuffer[0].size() != width)) {
            resize(width, height); // Resize depth buffer if enabling early Z-buffer
        }
    }
    void Rasterizer::enableBackFaceCulling(bool enable) {
        isEnableBackFaceCulling = enable; ///< Enable or disable back face culling
    }
    void Rasterizer::getViewportSize(int& width, int& height) const {
        width = this->width;
        height = this->height;
    }
} // Rasterizer