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
#include <utils/utils.h>

    Eigen::Matrix4d get_view_matrix(const Eigen::Vector3d& eye,
                                    const Eigen::Vector3d& center,
                                    const Eigen::Vector3d& up)
    {
        Eigen::Vector3d z = (eye - center).normalized(); // forward
        Eigen::Vector3d x = up.cross(z).normalized(); // right
        Eigen::Vector3d y = z.cross(x); // up

        Eigen::Matrix4d view = Eigen::Matrix4d::Identity();

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

    void task()
    {
        // 三角形
        Eigen::Vector4d v0(2, 0, -2, 1);
        Eigen::Vector4d v1(0, 2, -4, 1);
        Eigen::Vector4d v2(-2, 0, -2, 1);
        // 三角形绕轴旋转
        Eigen::Matrix4d rotate;
        double rotate_angle = 0.0 / 180 * 3.14159;
        rotate << cos(rotate_angle), -sin(rotate_angle), 0, 0,
            sin(rotate_angle), cos(rotate_angle), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;
        v0 = rotate * v0;
        v1 = rotate * v1;
        v2 = rotate * v2;

        // 相机坐标
        Eigen::Vector3d eye(0, 2, 2); // 相机位置
        Eigen::Vector3d center(0, 0, 0); // 相机看向目标
        Eigen::Vector3d up(0, 1, 0); // 上方向

        Eigen::Matrix4d view = get_view_matrix(eye, center, up);
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
        double half_height = tan(eye_fov  / 180 * M_PI / 2) * z_near;
        double half_width = half_height * aspect_ratio;

        // 映射到标准裁剪空间
        // 透视矩阵
        Eigen::Matrix4d mperspective;
        mperspective << z_near, 0, 0, 0,
            0, z_near, 0, 0,
            0, 0, z_near + z_far, z_near * z_far,
            0, 0, -1, 0;
        // 正交投影矩阵
        Eigen::Matrix4d morth;
        morth << 1 / half_width, 0, 0, 0,
            0, 1 / half_height, 0, 0,
            0, 0, 2 / (z_far - z_near), (z_far + z_near) / (z_far - z_near),
            0, 0, 0, 1;
        Eigen::Matrix4d morth_func = utils::MVP::cal_projection_matrix(eye_fov, aspect_ratio, z_near, z_far);
        // Eigen::Vector4d v0_standard = morth * mperspective * v0;
        // Eigen::Vector4d v1_standard = morth * mperspective * v1;
        // Eigen::Vector4d v2_standard = morth * mperspective * v2;
        Eigen::Vector4d v0_standard = morth_func * v0;
        Eigen::Vector4d v1_standard = morth_func * v1;
        Eigen::Vector4d v2_standard = morth_func * v2;
        // 透视除法
        v0_standard /= v0_standard.w();
        v1_standard /= v1_standard.w();
        v2_standard /= v2_standard.w();

        // 投影

        // 像素
        int screen_width = 512;
        int screen_length = 512;

        Eigen::Vector4d v0_screen = v0_standard;
        Eigen::Vector4d v1_screen = v1_standard;
        Eigen::Vector4d v2_screen = v2_standard;
        // opencv y坐标向下递增
        v0_screen(0) = (v0_screen(0) * screen_width) / 2;
        v0_screen(1) = (-v0_screen(1) * screen_length) / 2;
        v1_screen(0) = (v1_screen(0) * screen_width) / 2;
        v1_screen(1) = (-v1_screen(1) * screen_length) / 2;
        v2_screen(0) = (v2_screen(0) * screen_width) / 2;
        v2_screen(1) = (-v2_screen(1) * screen_length) / 2;
        std::cout << v0_screen.x() << " " << v0_screen.y() << " " << v0_standard.z() << std::endl;
        std::cout << v1_screen.x() << " " << v1_screen.y() << " " << v1_standard.z() << std::endl;
        std::cout << v2_screen.x() << " " << v2_screen.y() << " " << v2_standard.z() << std::endl;

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
        for (int i = min_y; i <= max_y; i++)
        {
            for (int j = min_x; j <= max_x; j++)
            {
                Eigen::Vector2i pixel(j, i);
                Eigen::Vector2i v0_pixel(static_cast<int>(v0_screen.x()), static_cast<int>(v0_screen.y()));
                Eigen::Vector2i v1_pixel(static_cast<int>(v1_screen.x()), static_cast<int>(v1_screen.y()));
                Eigen::Vector2i v2_pixel(static_cast<int>(v2_screen.x()), static_cast<int>(v2_screen.y()));
                if (cross(pixel - v1_pixel, v0_pixel - v1_pixel) < 0 && cross(pixel - v0_pixel, v2_pixel - v0_pixel) < 0
                    &&
                    cross(pixel - v2_pixel, v1_pixel - v2_pixel) < 0)
                {
                    SetPixel(j + screen_width / 2, i + screen_length / 2, Eigen::Vector3d(255, 0, 0));
                };
            }
        }
    }

/**
 * @brief rendering loop
 *
 */
inline void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
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
