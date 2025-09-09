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
#include <omp.h>

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
            lsr3d::TriangleData triangleData = triangle.second.toRawData(model.getTriangles(),
                                                                    model.getVertices(),
                                                                    model.getTextureCoords(),
                                                                    model.getNormals(),
                                                                    model.getMaterials());
            lsr3d::vertexInputData vertInputData{
                .triangle = triangleData,
                .M = model.getModelMatrix(),
                .VP = camera.getVPMatrix(),
                .width = width,
                .height = height
            };
            // vertex shader
            lsr3d::vertexOutputData vertOutputData;
            m_vertexShader.shading(vertInputData, vertOutputData);
            m_rasterizer.setFragmentShader(m_fragmentShader);
            m_rasterizer.rasterization(vertOutputData,
                model.getImages(),
                m_directionalLights, m_pointLights, m_spotLights);
        }
    }

    void Renderer::shutdown() {
    }
    ModelLoaderHandle Renderer::addModel(std::string modelPath) {
        ModelLoader modelLoader;
        if (modelLoader.loadModel(modelPath)) {
            m_models[ModelLoaderHandle(m_models.size())] = modelLoader;
            return ModelLoaderHandle(m_models.size() - 1); // 返回模型的句柄
        }
        return ModelLoaderHandle(); // 加载失败
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
    ModelLoader& Renderer::getModel(const ModelLoaderHandle& handle) {
        return m_models.at(handle);
    }
    DirectionalLightHandle Renderer::createDirectionalLight(lsr3d::DirectionalLight light) {
        // TODO: Implement directional light creation
        DirectionalLightHandle handle(m_directionalLights.size());
        m_directionalLights[handle] = light;
        return handle;
    }
    PointLightHandle Renderer::createPointLight(lsr3d::PointLight light) {
        // TODO: Implement point light creation
        PointLightHandle handle(m_pointLights.size());
        m_pointLights[handle] = light;
        return handle;
    }
    SpotLightHandle Renderer::createSpotLight(lsr3d::SpotLight light) {
        // TODO: Implement spot light creation
        SpotLightHandle handle(m_spotLights.size());
        m_spotLights[handle] = light;
        return handle;
    }
    lsr3d::DirectionalLight* Renderer::writeDirectionalLight(const DirectionalLightHandle& handle) {
        auto it = m_directionalLights.find(handle);
        if (it == m_directionalLights.end()) {
            return nullptr;
        }
        return &(it->second);
    }
    lsr3d::PointLight* Renderer::writePointLight(const PointLightHandle& handle) {
        auto it = m_pointLights.find(handle);
        if (it == m_pointLights.end()) {
            return nullptr;
        }
        return &(it->second);
    }
    lsr3d::SpotLight* Renderer::writeSpotLight(const SpotLightHandle& handle) {
        auto it = m_spotLights.find(handle);
        if (it == m_spotLights.end()) {
            return nullptr;
        }
        return &(it->second);
    }
    lsr3d::ModelLoader* Renderer::writeModel(const ModelLoaderHandle& handle) {
        auto it = m_models.find(handle);
        if (it == m_models.end()) {
            return nullptr;
        }
        return &(it->second);
    }
    const lsr3d::DirectionalLight* Renderer::readDirectionalLight(const DirectionalLightHandle& handle) const {
        auto it = m_directionalLights.find(handle);
        if (it == m_directionalLights.end()) {
            return nullptr;
        }
        return &(it->second);
    }
    const lsr3d::PointLight* Renderer::readPointLight(const PointLightHandle& handle) const {
        auto it = m_pointLights.find(handle);
        if (it == m_pointLights.end()) {
            return nullptr;
        }
        return &(it->second);
    }
    const lsr3d::SpotLight* Renderer::readSpotLight(const SpotLightHandle& handle) const {
        auto it = m_spotLights.find(handle);
        if (it == m_spotLights.end()) {
            return nullptr;
        }
        return &(it->second);
    }
    const lsr3d::ModelLoader* Renderer::readModel(const ModelLoaderHandle& handle) const {
        auto it = m_models.find(handle);
        if (it == m_models.end()) {
            return nullptr;
        }
        return &(it->second);
    }
}