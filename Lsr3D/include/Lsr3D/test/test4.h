/**
 * @file test2.h
 * @author dion (you@domain.com) zhywyt (zhywyt@yeah.net)
 * @brief finished task2 of GAMES101
 * @version 0.1
 * @date 2025-07-06
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#include <iostream>
#include <Lsr3D/utils/utils.h>
#include <Lsr3D/core/ModelLoader.h>
using namespace lsr3d;
class render;
class render {
public:
    static render instance;
    ModelLoader modelLoader;
    int width, height;
    std::vector<std::vector<float>> depthBuffer;
    render(int w=512, int h=512) {
        resize(w, h);
    }
    void resize(int w, int h) {
        width = w;
        height = h;
        // 先清空并预留空间
        depthBuffer.clear();
        depthBuffer.reserve(height);
        
        // 初始化每一行
        depthBuffer.resize(height);
        for (auto& row : depthBuffer) {
            row.reserve(width);
            row.resize(width, -std::numeric_limits<float>::max());
        }
        std::cout<<"resize depthBuffer to "<<width<<"x"<<height<<std::endl;
    }
    void clearDepthBuffer() {
        // 清空深度缓冲区，使用最小值表示最远距离
        for (auto& row : depthBuffer) {
            std::fill(row.begin(), row.end(), -std::numeric_limits<float>::max());
        }
        // std::cout<<"clear depthBuffer"<<std::endl;
    }
};
Eigen::Matrix4f get_view_matrix(const Eigen::Vector3f& eye,
    const Eigen::Vector3f& center,
    const Eigen::Vector3f& up) {
    Eigen::Vector3f z = (eye - center).normalized(); // forward
    Eigen::Vector3f x = up.cross(z).normalized(); // right
    Eigen::Vector3f y = z.cross(x); // up

    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    // 旋转部分（列向量：x, y, z）
    view.block<1, 3>(0, 0) = x.transpose();
    view.block<1, 3>(1, 0) = y.transpose();
    view.block<1, 3>(2, 0) = z.transpose();

    // 平移部分
    view(0, 3) = -x.dot(eye);
    view(1, 3) = -y.dot(eye);
    view(2, 3) = -z.dot(eye);

    return view;
}

// draw a triangle
void draw_triangle(const lsr3d::Triangle& triangle,const lsr3d::Material& material) {
    auto& vertex = render::instance.modelLoader.getVertices();
    Eigen::Vector4f v0 = vertex.at(triangle.v0);
    Eigen::Vector4f v1 = vertex.at(triangle.v1);
    Eigen::Vector4f v2 = vertex.at(triangle.v2);
    // 三角形绕轴旋转
    Eigen::Matrix4f rotate;
    float rotate_angle = 30.0f / 180 * 3.14159f;
    rotate << cos(rotate_angle), -sin(rotate_angle), 0, 0,
        sin(rotate_angle), cos(rotate_angle), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;
    v0 = rotate * v0;
    v1 = rotate * v1;
    v2 = rotate * v2;

    // 相机坐标
    Eigen::Vector3f eye(1, 2, 2); // 相机位置
    Eigen::Vector3f center(0, 0, 0); // 相机看向目标
    Eigen::Vector3f up(0, 1, 0); // 上方向

    Eigen::Matrix4f view = get_view_matrix(eye, center, up);
    v0 = view * v0;
    v1 = view * v1;
    v2 = view * v2;
    
    // 视角宽度（弧度度）
    double eye_fov = 100.0;
    // 宽高比
    double aspect_ratio = 1;
    // 认为相机朝-z方向看（右手系）但是z_near,z_far定义为离相机的距离
    double z_near = 2;
    double z_far = 4;
    double half_height = tan(eye_fov / 180 * M_PI / 2) * z_near;
    double half_width = half_height * aspect_ratio;

    // 映射到标准裁剪空间
    // 透视矩阵
    Eigen::Matrix4f mperspective;
    mperspective << z_near, 0, 0, 0,
        0, z_near, 0, 0,
        0, 0, z_near + z_far, z_near* z_far,
        0, 0, -1, 0;
    // 正交投影矩阵
    Eigen::Matrix4f morth;
    morth << 1 / half_width, 0, 0, 0,
        0, 1 / half_height, 0, 0,
        0, 0, 2 / (z_far - z_near), (z_far + z_near) / (z_far - z_near),
        0, 0, 0, 1;
    Eigen::Matrix4f morth_func = utils::MVP::cal_projection_matrix(eye_fov, aspect_ratio, z_near, z_far);
    // Eigen::Vector4f v0_standard = morth * mperspective * v0;
    // Eigen::Vector4f v1_standard = morth * mperspective * v1;
    // Eigen::Vector4f v2_standard = morth * mperspective * v2;
    Eigen::Vector4f v0_standard = morth_func * v0;
    Eigen::Vector4f v1_standard = morth_func * v1;
    Eigen::Vector4f v2_standard = morth_func * v2;
    // 透视除法
    v0_standard /= v0_standard.w();
    v1_standard /= v1_standard.w();
    v2_standard /= v2_standard.w();

    // 投影

    // 像素
    int screen_width = render::instance.width;
    int screen_length = render::instance.height;

    Eigen::Vector4f v0_screen = v0_standard;
    Eigen::Vector4f v1_screen = v1_standard;
    Eigen::Vector4f v2_screen = v2_standard;
    // opencv y坐标向下递增
    v0_screen(0) = (v0_screen(0) * screen_width) / 2;
    v0_screen(1) = (-v0_screen(1) * screen_length) / 2;
    v1_screen(0) = (v1_screen(0) * screen_width) / 2;
    v1_screen(1) = (-v1_screen(1) * screen_length) / 2;
    v2_screen(0) = (v2_screen(0) * screen_width) / 2;
    v2_screen(1) = (-v2_screen(1) * screen_length) / 2;
    // std::cout << v0_screen.x() << " " << v0_screen.y() << " " << v0_standard.z() << std::endl;
    // std::cout << v1_screen.x() << " " << v1_screen.y() << " " << v1_standard.z() << std::endl;
    // std::cout << v2_screen.x() << " " << v2_screen.y() << " " << v2_standard.z() << std::endl;

    // 获取纹理
    auto texturecoord = render::instance.modelLoader.getTextureCoords();
    lsr3d::TextureCoord uv0 = texturecoord.at(triangle.t0);
    lsr3d::TextureCoord uv1 = texturecoord.at(triangle.t1);
    lsr3d::TextureCoord uv2 = texturecoord.at(triangle.t2);
    const lsr3d::Image* image = nullptr;
    bool haveTextureImage = triangle.hasTextures && triangle.material_->imageHandle.is_valid();
    if(haveTextureImage) {
        image = &render::instance.modelLoader.getImages().at(triangle.material_->imageHandle);
    }
    // MSAA

    // 包围盒
    int min_x = std::min(std::min(static_cast<int>(v0_screen.x()), static_cast<int>(v1_screen.x())),
        static_cast<int>(v2_screen.x()));
    int max_x = std::max(std::max(static_cast<int>(v0_screen.x()), static_cast<int>(v1_screen.x())),
        static_cast<int>(v2_screen.x()));
    int min_y = std::min(std::min(static_cast<int>(v0_screen.y()), static_cast<int>(v1_screen.y())),
        static_cast<int>(v2_screen.y()));
    int max_y = std::max(std::max(static_cast<int>(v0_screen.y()), static_cast<int>(v1_screen.y())),
        static_cast<int>(v2_screen.y()));
    Eigen::Vector2i v0_pixel(static_cast<int>(v0_screen.x()), static_cast<int>(v0_screen.y()));
    Eigen::Vector2i v1_pixel(static_cast<int>(v1_screen.x()), static_cast<int>(v1_screen.y()));
    Eigen::Vector2i v2_pixel(static_cast<int>(v2_screen.x()), static_cast<int>(v2_screen.y()));
    #define FACE_CULL 1
    for (int i = min_y; i <= max_y; i++) {
        for (int j = min_x; j <= max_x; j++) {
            // 计算实际的屏幕坐标
            int screen_j = j + screen_width / 2;
            int screen_i = i + screen_length / 2;
            {   // 检查屏幕边界
                if (screen_i < 0 || screen_i >= screen_length || screen_j < 0 || screen_j >= screen_width) {
                    continue; // skip pixels outside the screen
                }
            }
            float area = cross(v1_pixel - v0_pixel, v2_pixel - v1_pixel);
            {   // 剔除操作
#if (FACE_CULL)
                // 面剔除
                // ? NOTE: There is some bugs, debug then seted the area > 0
                // camera look at -z direction, so if area > 0, it is back-facing
                if (area > 0) {
                    continue; // skip back-facing triangles
                }
#endif
                area = std::abs(area);
                if (area == 0) {
                    continue; // skip degenerate triangle
                }
            }
            Eigen::Vector2i pixel(j, i);
            Eigen::Vector3f barycentric;
            {   // 计算重心坐标
                barycentric.x() = cross(v1_pixel - pixel, v2_pixel - pixel) / area;
                barycentric.y() = cross(v2_pixel - pixel, v0_pixel - pixel) / area;
                barycentric.z() = cross(v0_pixel - pixel, v1_pixel - pixel) / area;
            }
            {   // 检查是否在三角形内
                if (barycentric.x() < 0 || barycentric.y() < 0 || barycentric.z() < 0) {
                    continue; // skip pixels outside the triangle
                }
            }
            float z = barycentric.x() * v0_screen.z() + barycentric.y() * v1_screen.z() + barycentric.z() * v2_screen.z();
            {   // 深度测试
                // 更近的物体（更大的z值，因为view space中z是负的）
                if (z < render::instance.depthBuffer[screen_i][screen_j]) {
                    continue;
                }
            }
            lsr3d::Color color(barycentric.x(), barycentric.y(), barycentric.z(), 1.0f); // 红色作为默认颜色
            {   // 纹理采样
                if (haveTextureImage) {
                    lsr3d::TextureCoord uv;
                    uv.u() = barycentric.x() * uv0.u() + barycentric.y() * uv1.u() + barycentric.z() * uv2.u();
                    uv.v() = barycentric.x() * uv0.v() + barycentric.y() * uv1.v() + barycentric.z() * uv2.v();
                    color = image->SampleNearest(uv.u(), uv.v());
                    // color = image->SampleLinear(uv.u(), uv.v());
                    color = color / 255.0f;
                }
            }
            // 更新深度缓冲区
            render::instance.depthBuffer[screen_i][screen_j] = z;
            SetPixel(screen_j, screen_i, color);
        }
    }
}

void task() {
    const auto& vertices = render::instance.modelLoader.getVertices();
    const auto& triangles = render::instance.modelLoader.getTriangles();
    for (auto& triangle_ : triangles) {
        auto& triangle = triangle_.second;
        draw_triangle(triangle, 
            render::instance.modelLoader.getMaterials().at(triangle.materialName));
    }
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
    render::instance.modelLoader.loadModel("assets/model/cube/cube.obj");
}
