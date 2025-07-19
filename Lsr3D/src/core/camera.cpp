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

#include <Lsr3D/core/camera.h>

camera::camera(const Eigen::Vector3f& position, const Eigen::Vector3f& up, float yaw, float pitch)
    : position(position)
      , world_up(up)
      , yaw(yaw)
      , pitch(pitch)
      , movement_speed(2.5f)
      , mouse_sensitivity(0.1f)
      , zoom(45.0f)
      , front(Eigen::Vector3f(0.0f, 0.0f, -1.0f))
{
    update_camera_vectors();
}


void camera::update_camera_vectors()
{
    // 计算相机前方向
    Eigen::Vector3f front;
    front.x() = cos(utils::deg2rad(yaw)) * cos(utils::deg2rad(pitch));
    front.y() = sin(utils::deg2rad(pitch));
    front.z() = sin(utils::deg2rad(yaw)) * cos(utils::deg2rad(pitch));
    this->front = front.normalized();
    // 计算相机右方向
    this->right = this->front.cross(this->world_up).normalized();
    // 计算相机上方向
    this->up = this->right.cross(this->front).normalized();
}
