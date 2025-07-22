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
#include <Lsr3D/utils/MVP.h>
#include <Lsr3D/utils/utils.h>

class Camera
{
public:
    // 核心数据 - 计算VP矩阵的最小必需数据
    Eigen::Vector3f position;  // 相机位置
    Eigen::Vector3f target;    // 相机看向的目标点
    Eigen::Vector3f up;        // 相机上方向
    float fov;                 // 视场角
    float aspect;              // 宽高比
    float zNear;               // 近裁剪面
    float zFar;                // 远裁剪面

    // 构造函数 - 只需要最少的参数
    Camera(const Eigen::Vector3f& position = Eigen::Vector3f(0.0f, 0.0f, 3.0f),
           const Eigen::Vector3f& target = Eigen::Vector3f(0.0f, 0.0f, 0.0f),
           const Eigen::Vector3f& up = Eigen::Vector3f(0.0f, 1.0f, 0.0f),
           float fov = 45.0f,
           float aspect = 4.0f/3.0f,
           float zNear = 0.1f,
           float zFar = 100.0f);

    // 快速获取VP矩阵
    inline Eigen::Matrix4f getVPMatrix() const {
        if (vp_dirty) {
            update_vp_matrix();
        }
        return vp_matrix;
    }

    // 快速获取MVP矩阵
    inline Eigen::Matrix4f getMVPMatrix(const Eigen::Matrix4f& modelMatrix) const {
        return getVPMatrix() * modelMatrix;
    }

    // 设置相机位置（自动标记VP矩阵需要更新）
    void setPosition(const Eigen::Vector3f& pos);
    void setTarget(const Eigen::Vector3f& tgt);
    void setUp(const Eigen::Vector3f& up_vec);
    void setProjection(float fov, float aspect, float near_plane, float far_plane);

    // 便捷的相机控制方法
    void lookAt(const Eigen::Vector3f& eye, const Eigen::Vector3f& center, const Eigen::Vector3f& up_vec);
    void setViewport(int width, int height);

private:
    mutable Eigen::Matrix4f vp_matrix;  // 缓存的VP矩阵
    mutable bool vp_dirty = true;       // VP矩阵是否需要更新

    void update_vp_matrix() const;
};


#endif //CAMERA_H
