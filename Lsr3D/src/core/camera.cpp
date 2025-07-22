/**
 * @file camera.cpp
 * @author dion (hduer_zdy@outlook.com)
 * @brief 重构后的Camera类实现
 * @version 0.2
 * @date 2025/7/22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <Lsr3D/core/camera.h>

Camera::Camera(const Eigen::Vector3f& position, const Eigen::Vector3f& target, 
               const Eigen::Vector3f& up, float fov, float aspect, 
               float zNear, float zFar)
    : position(position)
    , target(target)
    , up(up)
    , fov(fov)
    , aspect(aspect)
    , zNear(zNear)
    , zFar(zFar)
    , vp_dirty(true)
{
}

void Camera::setPosition(const Eigen::Vector3f& pos) {
    position = pos;
    vp_dirty = true;
}

void Camera::setTarget(const Eigen::Vector3f& tgt) {
    target = tgt;
    vp_dirty = true;
}

void Camera::setUp(const Eigen::Vector3f& up_vec) {
    up = up_vec;
    vp_dirty = true;
}

void Camera::setProjection(float fov_val, float aspect_val, float near_plane, float far_plane) {
    fov = fov_val;
    aspect = aspect_val;
    zNear = near_plane;
    zFar = far_plane;
    vp_dirty = true;
}

void Camera::lookAt(const Eigen::Vector3f& eye, const Eigen::Vector3f& center, const Eigen::Vector3f& up_vec) {
    position = eye;
    target = center;
    up = up_vec;
    vp_dirty = true;
}

void Camera::setViewport(int width, int height) {
    aspect = static_cast<float>(width) / static_cast<float>(height);
    vp_dirty = true;
}

void Camera::update_vp_matrix() const {
    // 计算视图矩阵
    Eigen::Matrix4f view = utils::MVP::cal_view_matrix(position, target, up);
    // 计算投影矩阵
    Eigen::Matrix4f projection = utils::MVP::cal_projection_matrix(fov, aspect, zNear, zFar);
    // 计算并缓存VP矩阵
    vp_matrix = projection * view;
    vp_dirty = false;
}
