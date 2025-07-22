/**
 * @file render.h
 * @author zhywyt (zhywyt@yeah.net)
 * @brief use shader and rasterizer to render
 * @version 0.1
 * @date 2025-07-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once
#include <Lsr3D/core/shader.h>
#include <Lsr3D/core/Rasterizer.h>
#include <Lsr3D/core/camera.h>
#include <lsr3d/core/ModelLoader.h>
#include <string>
#include <vector>
#include <unordered_map>

using vertShader = lsr3d::triangleVertexShader;
using fragShader = lsr3d::triangleFragmentShader;

namespace lsr3d
{
    class Renderer
    {
    public:
        static Renderer instance;
        Renderer(int width, int height);
        ~Renderer();
        ModelHandle addModel(std::string modelPath);
        ModelLoader& getModel(const ModelHandle& handle);

        void initialize();
        void shutdown();
        void getViewportSize(int& width, int& height)const;
        void resize(int width, int height);

        void renderScene(const Camera& camera);
        void renderModel(const ModelLoader& model, const Camera& camera);

        void clearDepthBuffer();

    private:
        lsr3d::triangleVertexShader m_vertexShader;
        lsr3d::triangleFragmentShader m_fragmentShader;
        lsr3d::Rasterizer m_rasterizer;
        std::unordered_map<ModelHandle, lsr3d::ModelLoader> m_models;
    };
}