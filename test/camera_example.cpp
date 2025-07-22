/**
 * @file camera_example.cpp
 * @author dion (hduer_zdy@outlook.com)
 * @brief Camera类使用示例 - 展示如何用最少数据获取VP矩阵
 * @version 0.2
 * @date 2025/7/22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <Lsr3D/core/camera.h>
#include <iostream>

int main() {
    // 示例1: 使用最少的数据创建相机
    // 只需要7个核心参数，而不是之前的10多个参数
    Camera camera(
        Eigen::Vector3f(0, 0, 5),    // 相机位置
        Eigen::Vector3f(0, 0, 0),    // 目标点
        Eigen::Vector3f(0, 1, 0),    // 上方向
        45.0f,                       // 视场角
        16.0f/9.0f,                  // 宽高比
        0.1f,                        // 近裁剪面
        100.0f                       // 远裁剪面
    );

    // 示例2: 获取VP矩阵 - 只需一行代码
    Eigen::Matrix4f vp_matrix = camera.getVPMatrix();
    
    // 示例3: 获取MVP矩阵 - 传入模型矩阵即可
    Eigen::Matrix4f model_matrix = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f mvp_matrix = camera.getMVPMatrix(model_matrix);

    // 示例4: 动态更新相机参数（自动触发VP矩阵重新计算）
    camera.setPosition(Eigen::Vector3f(0, 2, 8));
    camera.setTarget(Eigen::Vector3f(0, 0, -2));
    
    // VP矩阵会在下次访问时自动重新计算（延迟计算优化）
    Eigen::Matrix4f updated_vp = camera.getVPMatrix();

    // 示例5: 设置视口（自动更新宽高比）
    camera.setViewport(1920, 1080);
    
    // 示例6: 使用lookAt快速设置相机
    camera.lookAt(
        Eigen::Vector3f(10, 10, 10),  // 眼睛位置
        Eigen::Vector3f(0, 0, 0),     // 看向目标
        Eigen::Vector3f(0, 1, 0)      // 上方向
    );

    std::cout << "重构后的Camera类特点：\n";
    std::cout << "1. 只需7个核心数据成员（之前需要15+个）\n";
    std::cout << "2. VP矩阵自动缓存，避免重复计算\n";
    std::cout << "3. 延迟计算，只在需要时更新\n";
    std::cout << "4. 接口简洁，一行代码获取VP/MVP矩阵\n";
    std::cout << "5. 内存使用更少，性能更高\n";

    return 0;
}
