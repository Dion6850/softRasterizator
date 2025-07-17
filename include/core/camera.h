/**
 * @file camera.h
 * @author dion (hduer_zdy@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2025/7/15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef CAMERA_H
#define CAMERA_H
#include "utils/MVP.h"
#include "utils/utils.h"

class camera
{
public:
    // 相机属性
    Eigen::Vector3f position; // 相机位置
    Eigen::Vector3f world_up; // 世界坐标系的上方向

    // 欧拉角
    float yaw; // 偏航角
    float pitch; // 俯仰角

    // 相机参数
    float movement_speed; // 移动速度
    float mouse_sensitivity; // 鼠标灵敏度
    float zoom; // 视野角度

    Eigen::Vector3f front; // 相机前方向
    Eigen::Vector3f up; // 相机上方向
    Eigen::Vector3f right; // 相机右方向

    // 构造函数
    camera(const Eigen::Vector3f& position = Eigen::Vector3f(0.0f, 0.0f, 0.0f),
           const Eigen::Vector3f& up = Eigen::Vector3f(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f,
           float pitch = 0.0f);

private:
    void update_camera_vectors();
};


#endif //CAMERA_H
