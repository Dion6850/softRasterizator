/**
 * @file utils.h
 * @author zhywyt (zhywyt@yeah.net)
 * @brief
 * @version 0.1
 * @date 2025-07-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#include <GL/freeglut.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Lsr3D/utils/MVP.h>
#include <Lsr3D/utils/math_utils.h>
#include <Lsr3D/core/resource.h>
#include <Lsr3D/core/buffer.h>
extern FrameBuffer defaultBuffer;
extern int initialWidth;
extern int initialHeight;
namespace lsr3d
{
    /**
     * @brief Set Pixel Color
     * @param x x coordinate
     * @param y y coordinate
     * @param color color vector (1.0,1.0,1.0)
     * @details The coordinate system takes the top-left corner of the screen as the origin, with x going right and y going down.
     * @warning Must be wrapped in glBegin(GL_POINTS) and glEnd().
     */
    void SetPixel(const int& x, const int& y, const Eigen::Vector3d& color);
    void SetPixel(const int& x, const int& y, const lsr3d::Color& color);

    int cross(Eigen::Vector2i a, Eigen::Vector2i b);
    float cross2F(Eigen::Vector2f a, Eigen::Vector2f b);
    template<typename T>
    T mix(const T& a, const T& b, float factor) {
        return a * (1.0f - factor) + b * factor;
    }
    template<typename T>
    Eigen::Vector3f mix(const Eigen::Vector3f a, const Eigen::Vector3f b, float factor) {
        return a * (1.0f - factor) + b * factor;
    }
    template<typename T>
    T pow5(const T& x) {
        T x2 = x * x;
        return x2 * x2 * x;
    }
}
namespace utils
{
    /**
     * @brief 角度转换
     * @param degrees 度
     * @return 弧度
     */
    constexpr double deg2rad(double degrees) {
        return degrees * M_PI / 180.0;
    }

    /**
     * @brief 角度转换
     * @param radians 弧度
     * @return 度
     */
    constexpr double rad2deg(double radians) {
        return radians * 180.0 / M_PI;
    }

    // #include <cmath>
    /*
    数值微分算法实现
    */
    /// <summary>
    /// digitial differential analyzer
    /// called DAA
    /// </summary>
    /// <param name="startx">起始位置x</param>
    /// <param name="starty"></param>
    /// <param name="endx">终止位置x</param>
    /// <param name="endy"></param>
    void DDA_Line(GLint startx, GLint starty, GLint endx, GLint endy);

    /*
    中点画线算法实现
    */
    /// <summary>
    /// The Middle Point
    /// Called TMP
    /// </summary>
    /// <param name="startx">起始位置x</param>
    /// <param name="starty"></param>
    /// <param name="endx">终止位置x</param>
    /// <param name="endy"></param>
    void TMP_Line(GLint startx, GLint starty, GLint endx, GLint endy);


    /*
    Bresenham算法
    这是图形学中用的最多的直线生成算法，全部是整数计算，加快了计算的速度
    */
    /// <summary>
    /// Bresenham
    /// </summary>
    /// <param name="startx"></param>
    /// <param name="starty"></param>
    /// <param name="endx"></param>
    /// <param name="endy"></param>
    void BRESENHAM_Line(GLint startx, GLint starty, GLint endx, GLint endy);

}

