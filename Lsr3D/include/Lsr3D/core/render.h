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
    class Renderer {
    public:
        static Renderer instance;
        Renderer(int width, int height);
        ~Renderer();
        ModelLoaderHandle addModel(std::string modelPath);
        ModelLoader& getModel(const ModelLoaderHandle& handle);

        void initialize();
        void shutdown();
        void getViewportSize(int& width, int& height)const;
        void resize(int width, int height);

        /**
         * @brief Render the entire scene
         *
         * @param camera restore shade pipeline data
         * @warning TODO: restore the shade pipeline data with camera
         * @warning TODO: support fragment buffer
         * @details This function will call the vertex shader and fragment shader to render the scene.
         */
        void renderScene(const Camera& camera);
        /**
         * @brief Render a single 3D Model
         *
         * @param model
         * @param camera
         */
        void renderModel(const ModelLoader& model, const Camera& camera);

        void clearDepthBuffer();

        /**
         * @brief Create a Directional Light object
         *
         * @param light
         * @return light handle
         */
        DirectionalLightHandle createDirectionalLight(lsr3d::DirectionalLight light);
        PointLightHandle createPointLight(lsr3d::PointLight light);
        SpotLightHandle createSpotLight(lsr3d::SpotLight light);


        /**
         * @brief the write function can get pointer to internal data for modification
         *
         * @param handle 
         * @return pointer to internal data, nullptr if handle not found
         * @example
         * ```cpp
         * if(Light* light = render.writeDirectionalLight(handle); light != nullptr){
         *     // modify light
         * }
         * ```
         */
        lsr3d::DirectionalLight* writeDirectionalLight(const DirectionalLightHandle& handle);
        lsr3d::PointLight* writePointLight(const PointLightHandle& handle);
        lsr3d::SpotLight* writeSpotLight(const SpotLightHandle& handle);
        lsr3d::ModelLoader* writeModel(const ModelLoaderHandle& handle);

        /**
         * @brief the read function can get const pointer to internal data for reading
         * 
         * @param handle 
         * @return const pointer to internal data, nullptr if handle not found
         * @example
         * ```cpp
         * if(const Light* light = render.readDirectionalLight(handle); light != nullptr){
         *     // use light
         * }
         * ```
         */
        const lsr3d::DirectionalLight* readDirectionalLight(const DirectionalLightHandle& handle)const;
        const lsr3d::PointLight* readPointLight(const PointLightHandle& handle)const;
        const lsr3d::SpotLight* readSpotLight(const SpotLightHandle& handle)const;
        const lsr3d::ModelLoader* readModel(const ModelLoaderHandle& handle)const;
    private:
        lsr3d::triangleVertexShader m_vertexShader;
        lsr3d::triangleFragmentShader m_fragmentShader;
        lsr3d::Rasterizer m_rasterizer;
        ModelLoaderDatas m_models;
        DirectionalLightDatas m_directionalLights;
        PointLightDatas m_pointLights;
        SpotLightDatas m_spotLights;
    };
}