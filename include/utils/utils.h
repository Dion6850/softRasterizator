#ifndef  UTILS_H
#define UTILS_
#include <GL/freeglut.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <utils/MVP.h>
/**
 * @brief Set Pixel Color
 * @param x x coordinate
 * @param y y coordinate
 * @param color color vector (255,255,255)
 * @details The coordinate system takes the top-left corner of the screen as the origin, with x going right and y going down.
 * @warning Must be wrapped in glBegin(GL_POINTS) and glEnd().
 */
inline void SetPixel(const int& x, const int& y, const Eigen::Vector3d& color)
{
    glColor3f(color.x(), color.y(), color.z());
    glVertex2i(x, y);
}

inline int cross(Eigen::Vector2i a, Eigen::Vector2i b)
{
    return a.x() * b.y() - a.y() * b.x();
}

namespace utils
{
    /**
     * @brief 角度转换
     * @param degrees 度
     * @return 弧度
     */
    constexpr double deg2rad(double degrees)
    {
        return degrees * M_PI / 180.0;
    }

    /**
     * @brief 角度转换
     * @param radians 弧度
     * @return 度
     */
    constexpr double rad2deg(double radians)
    {
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
#endif
