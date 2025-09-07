/**
 *@file test6.h
 * @author zhywyt (zhywyt@yeah.net)
 * @brief test lighting and phong shading
 * @version 0.1
 * @date 2025-07-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#include <iostream>
#include <Lsr3D/utils/utils.h>
#include <Lsr3D/utils/MVP.h>
#include <Lsr3D/core/render.h>
#include <Lsr3D/core/Camera.h>
#include <Lsr3D/core/ModelLoader.h>
using namespace lsr3d;
using render = lsr3d::Renderer;
void task() {
    int width, height;
    render::instance.getViewportSize(width, height);
    float scale = 1.0f;
    Camera m_camera(
        Eigen::Vector3f(2.0f * scale, 2.0f * scale, 4.0f * scale), // 相机位置
        Eigen::Vector3f(0.0f, 0.0f, 0.0f), // 目标点
        Eigen::Vector3f(0.0f, 1.0f, 0.0f), // 上方向
        90.0f,                            // 视场角
        static_cast<float>(width) / height, // 宽高比
        0.1f,                             // 近裁剪面
        100.0f                            // 远裁剪面
    );
    render::instance.renderScene(m_camera);
}

void Init(int width, int height) {

    render::instance.initialize();
    // ModelLoaderHandle modelHandle = render::instance.addModel("assets/model/cube/cube.obj");
    ModelLoaderHandle modelHandle = render::instance.addModel("assets/model/bunny/bunny.obj");
    ModelLoader& modelLoader = render::instance.getModel(modelHandle);
    render::instance.getModel(modelHandle).setModelMatrix(
        utils::MVP::cal_model_matrix(
            Eigen::Vector3f(0.0f, 0.0f, 0.0f), // 平移
            Eigen::Vector3f(0.0f, 0.0f, 0.0f), // 旋转
            Eigen::Vector3f(2.0f, 2.0f, 2.0f)  // 缩放
        )
    );

    // create test directional light

    // create test spot light
    SpotLightHandle spotLightHandle = render::instance.createSpotLight(
        SpotLight(
            Eigen::Vector4f(0.0f, 0.0f, -1.0f, 1.0f), // 光源位置
            Eigen::Vector4f(0.0f, 0.0f, 1.0f, 0.0f), // 光源方向
            Eigen::Vector4f(1.0f, 0.0f, 0.0f, 1.0f), // 光源颜色
            15.0f, // 聚光灯角度
            0.5f   // 光源强度
        )
    );
    // create test point light
    PointLightHandle pointLightHandle = render::instance.createPointLight(
        PointLight(
            Eigen::Vector4f(0.0f, 0.0f, -1.0f, 1.0f), // 光源位置
            Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f), // 光源颜色
            0.2f // 光源强度
        )
    );

}
