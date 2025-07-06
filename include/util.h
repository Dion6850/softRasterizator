#pragma once
#include <GL/freeglut.h>
#include <Eigen/Dense>
/**
 * @brief Set Pixel Color
 * @param x x coordinate
 * @param y y coordinate
 * @param color color vector (255,255,255)
 * @details The coordinate system takes the top-left corner of the screen as the origin, with x going right and y going down.
 * @warning Must be wrapped in glBegin(GL_POINTS) and glEnd().
 */
inline void SetPixel(const int& x, const int&y, const Eigen::Vector3d& color) {
    glColor3f(color.x(), color.y(), color.z());
    glVertex2i(x, y);
}

inline int cross(Eigen::Vector2i a, Eigen::Vector2i b) {
    return a.x() * b.y() - a.y() * b.x();
}

