#pragma once
#include <GL/freeglut.h>
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
inline void SetPixel(const int& x, const int& y, const Eigen::Vector3d& color) {
    glColor3f(color.x(), color.y(), color.z());
    glVertex2i(x, y);
}

inline int cross(Eigen::Vector2i a, Eigen::Vector2i b) {
    return a.x() * b.y() - a.y() * b.x();
}

namespace utils
{
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
    void DDA_Line(GLint startx, GLint starty, GLint endx, GLint endy) {
        glBegin(GL_POINTS);
        if (startx != endx && starty != endy) {
            double x, y;
            double k; // 斜率
            k = (static_cast<float>(endy - starty) / static_cast<float>(endx - startx));
            x = (double)startx, y = (double)starty;
            if (abs(k) < 1.0) {
                for (GLint i = 0; i < abs(endx - startx); i++) {
                    if (endx > startx) {
                        x += 1;
                        y += k;
                    } else {
                        x -= 1;
                        y -= k;
                    }
                    glVertex2i(static_cast<int>(x), static_cast<int>(y + 0.5)); // y四舍五入
                }
            } else if (abs(k) > 1.0) {
                for (GLint i = 0; i < abs(endy - starty); i++) {
                    if (endy > starty) {
                        y += 1;
                        x += 1.0 / k;
                    } else {
                        y -= 1;
                        x -= 1.0 / k;
                    }
                    glVertex2i(static_cast<int>(x + 0.5), static_cast<int>(y)); // x四舍五入
                }
            }
        } else if (startx == endx) { // 垂直画线
            if (starty < endy) {
                for (GLint i = starty; i < endy; i++) {
                    glVertex2i(startx, i);
                }
            } else if (starty > endy) {
                for (GLint i = endy; i < starty; i++) {
                    glVertex2i(startx, i);
                }
            }
        } else if (starty == endy) { // 水平画线
            if (startx < endx) {
                for (GLint i = startx; i < endx; i++) {
                    glVertex2i(i, starty);
                }
            } else if (startx > endx) {
                for (GLint i = endx; i < startx; i++) {
                    glVertex2i(i, starty);
                }
            }
        }
        glFlush();
        glEnd();
    }

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
    void TMP_Line(GLint startx, GLint starty, GLint endx, GLint endy) {
        glBegin(GL_POINTS);
        if (startx != endx && starty != endy) {
            bool kFlag = 0;  // 斜率是否大于1
            GLint sFlag = 1; // 斜率的正负
            if (startx > endx) { // 因为算法是x递增的，所以要保持起点x在终点左边
                GLint tempx = startx, tempy = starty;
                startx = endx, starty = endy;
                endx = tempx, endy = tempy;
            }
            if (abs(starty - endy) > abs(startx - endx)) {
                kFlag = true;
            }
            if (starty > endy) { // 标记斜率小于零
                sFlag = -1;
            }
            GLint a, b, TA, TAB, d, x, y;
            if (sFlag == -1)
                endy = starty + (starty - endy);

            a = starty - endy;
            b = endx - startx;
            TA = 2 * a;        // twoA
            TAB = 2 * (a + b); // twoAB
            d = 2 * a + b;
            x = startx, y = starty;
            if (!kFlag) {
                for (GLint i = 0; i < (endx - startx); i++) {
                    if (d >= 0) {
                        glVertex2i(++x, y);
                        d += TA;
                    } else {
                        glVertex2i(++x, y += sFlag);
                        d += TAB;
                    }
                }
            } else if (kFlag) {
                if (kFlag == 1) {
                    TA = 2 * b;
                    d = 2 * b + a;
                }
                for (GLint i = 0; i < abs(endy - starty); i++) {
                    if (d >= 0) {
                        glVertex2i(++x, y += sFlag);
                        d += TAB;
                    } else {
                        glVertex2i(x, y += sFlag);
                        d += TA;
                    }
                }
            }
        } else if (startx == endx) { // 垂直画线
            if (starty < endy) {
                for (GLint i = starty; i < endy; i++) {
                    glVertex2i(startx, i);
                }
            } else if (starty > endy) {
                for (GLint i = endy; i < starty; i++) {
                    glVertex2i(startx, i);
                }
            }
        } else if (starty == endy) { // 水平画线
            if (startx < endx) {
                for (GLint i = startx; i < endx; i++) {
                    glVertex2i(i, starty);
                }
            } else if (startx > endx) {
                for (GLint i = endx; i < startx; i++) {
                    glVertex2i(i, starty);
                }
            }
        }
        glFlush();
        glEnd();
    }

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
    void BRESENHAM_Line(GLint startx, GLint starty, GLint endx, GLint endy) {
        glBegin(GL_POINTS);
        if (startx != endx && starty != endy) {
            bool kFlag = 0;  // 斜率是否大于1
            GLint sFlag = 1; // 斜率的正负
            if (startx > endx) { // 因为算法是x递增的，所以要保持起点x在终点左边
                GLint tempx = startx, tempy = starty;
                startx = endx, starty = endy;
                endx = tempx, endy = tempy;
            }
            if (abs(starty - endy) > abs(startx - endx)) {
                kFlag = true;
            }
            if (starty > endy) { // 标记斜率小于零
                sFlag = -1;
            }
            GLint x, y;
            GLint dx, dy, p;
            if (sFlag == -1)
                endy = starty + (starty - endy);
            dx = endx - startx;
            dy = endy - starty;
            x = startx, y = starty;
            if (!kFlag) {
                p = 2 * dy - dx;
                for (GLint i = 0; i < (endx - startx); i++) {
                    if (p <= 0) {
                        glVertex2i(++x, y);
                        p += 2 * dy;
                    } else {
                        glVertex2i(++x, y += sFlag);
                        p += 2 * dy - 2 * dx;
                    }
                }
            } else {
                p = 2 * dx - dy;
                for (GLint i = 0; i < (endy - starty); i++) {
                    if (p <= 0) {
                        glVertex2i(x, y += sFlag);
                        p += 2 * dx;
                    } else {
                        glVertex2i(++x, y += sFlag);
                        p += 2 * dx - 2 * dy;
                    }
                }
            }
        } else if (startx == endx) { // 垂直画线
            if (starty < endy) {
                for (GLint i = starty; i < endy; i++) {
                    glVertex2i(startx, i);
                }
            } else if (starty > endy) {
                for (GLint i = endy; i < starty; i++) {
                    glVertex2i(startx, i);
                }
            }
        } else if (starty == endy) { // 水平画线
            if (startx < endx) {
                for (GLint i = startx; i < endx; i++) {
                    glVertex2i(i, starty);
                }
            } else if (startx > endx) {
                for (GLint i = endx; i < startx; i++) {
                    glVertex2i(i, starty);
                }
            }
        }
        glFlush();
        glEnd();
    }
}