/**
 *@file test5.h
 * @author zhywyt (zhywyt@yeah.net)
 * @brief test render class
 * @version 0.1
 * @date 2025-07-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once
#include <iostream>
#include <Lsr3D/utils/utils.h>
#include <Lsr3D/core/render.h>
#include <Lsr3D/core/Camera.h>
#include <Lsr3D/core/ModelLoader.h>
using namespace lsr3d;
using render = lsr3d::Renderer;
void task() {
    int width, height;
    render::instance.getViewportSize(width, height);
    Camera m_camera(
        Eigen::Vector3f(4.0f, 2.0f, 5.0f), // 相机位置
        Eigen::Vector3f(0.0f, 0.0f, 0.0f), // 目标点
        Eigen::Vector3f(0.0f, 1.0f, 0.0f), // 上方向
        90.0f,                            // 视场角
        static_cast<float>(width) / height, // 宽高比
        0.1f,                             // 近裁剪面
        100.0f                            // 远裁剪面
    );
    render::instance.renderScene(m_camera);
}
/**
 * @brief rendering loop
 *
 */
inline void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    render::instance.clearDepthBuffer(); // 每帧清空深度缓冲区
    glBegin(GL_POINTS);
    /*
    ################## just can draw points ##################
    */

    task();

    /*
    ################## just can draw points ##################
    */
    glEnd();
    glFlush();
}

void Init(int width, int height) {

    render::instance.initialize();
    ModelHandle modelHandle = render::instance.addModel("assets/model/cube/cube.obj");
    // ModelHandle modelHandle = render::instance.addModel("assets/model/bunny/bunny.obj");
    ModelLoader& modelLoader = render::instance.getModel(modelHandle);

}
