/**
 * @file MVP.h
 * @author dion (hduer_zdy@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2025-07-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef SOFTRESTERIZATOR_MVP_H
#define SOFTRESTERIZATOR_MVP_H

#include <Eigen/Dense>
#define M_PI 3.1415926

namespace utils
{
    class MVP
    {
    public:
        MVP() = delete;
        MVP(const MVP&) = delete;
        MVP& operator=(const MVP&) = delete;
        /**
         * @brief 计算相机视角变换矩阵
         * @param eye 相机位置
         * @param center 相机看向的目标
         * @param up 上方向
         * @return 相机视角变换矩阵
         */
        static Eigen::Matrix4f cal_view_matrix(const Eigen::Vector3f& eye,
                                               const Eigen::Vector3f& center,
                                               const Eigen::Vector3f& up);

        /**
         * @brief 计算模型变换矩阵
         * @param translation 位移向量
         * @param rotation 旋转向量
         * @param scale 缩放向量
         * @return 模型变换矩阵
         */
        static Eigen::Matrix4f cal_model_matrix(const Eigen::Vector3f& translation,
                                                const Eigen::Vector3f& rotation,
                                                const Eigen::Vector3f& scale);
        /**
         * @brief 计算相机视角的正交变换矩阵
         * @param fov 视场角（高）
         * @param aspect 视场角宽高比
         * @param near 近平面离相机的距离
         * @param far 原平面离相机的距离
         * @return 正交变换矩阵
         */
        static Eigen::Matrix4f cal_projection_matrix(const float& fov,
                                                     const float& aspect,
                                                     const float& near,
                                                     const float& far);
    };
}


#endif //SOFTRESTERIZATOR_MVP_H
