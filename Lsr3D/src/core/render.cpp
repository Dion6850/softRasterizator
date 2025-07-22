/**
 * @file render.cpp
 * @author zhywyt (zhywyt@yeah.net)
 * @brief 
 * @version 0.1
 * @date 2025-07-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <lsr3d/core/render.h>


namespace lsr3d
{
    Renderer::Renderer(int width, int height) : m_vertexShader(vertShader()), m_fragmentShader(fragShader()), m_rasterizer(width, height) {}
    Renderer::~Renderer() {}

    void Renderer::initialize() {
    }

    void Renderer::renderScene(const Camera& camera) {
        // render all models
        for (const auto& model : m_models) {
            renderModel(model.second, camera);
        }
    }
    void Renderer::renderModel(const ModelLoader& model, const Camera& camera) {
        // render a single model
        int width, height;
        m_rasterizer.getViewportSize(width, height);
        for (const auto& triangle : model.getTriangles()) {
            lsr3d::vertexInputData vertInputData{
                triangle.second.toRawData(model.getTriangles(),
                                          model.getVertices(),
                                          model.getTextureCoords(),
                                          model.getNormals()),
                camera.getMVPMatrix(model.getModelMatrix()),
                width, height // Pass viewport size for screen space conversion
            };
            // vertex shader
            lsr3d::vertexOutputData vertOutputData;
            m_vertexShader.shading(vertInputData, vertOutputData);
            m_rasterizer.setFragmentShader(m_fragmentShader);
            m_rasterizer.rasterization(vertOutputData, model.getImages());
        }
    }

    void Renderer::shutdown() {
    }
    ModelHandle Renderer::addModel(std::string modelPath) {
        ModelLoader modelLoader;
        if (modelLoader.loadModel(modelPath)) {
            m_models[ModelHandle(m_models.size())] = modelLoader;
            return ModelHandle(m_models.size() - 1); // 返回模型的句柄
        }
        return ModelHandle(); // 加载失败
    }
    void Renderer::getViewportSize(int& width, int& height) const {
        m_rasterizer.getViewportSize(width, height);
    }
    void Renderer::resize(int width, int height) {
        m_rasterizer.resize(width, height);
    }
    void Renderer::clearDepthBuffer() {
        m_rasterizer.clearDepthBuffer();
    }
    ModelLoader& Renderer::getModel(const ModelHandle& handle) {
        return m_models.at(handle);
    }

}