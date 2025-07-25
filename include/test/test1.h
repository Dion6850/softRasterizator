/**
 * @file test1.h
 * @author dion (you@domain.com) zhywyt (zhywyt@yeah.net)
 * @brief finished task 1 of GAMES101
 * @version 0.1
 * @date 2025-07-06
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#include <utils/utils.h>

    /**
     * @brief set rotation matrix
     *
     * @return Eigen::Vector3d
     */
    Eigen::Vector3d task1() {
        Eigen::Vector3d P(2, 1, 1);
        float theta = 0;
        Eigen::Matrix3d mat;
        mat << cos(theta), -sin(theta), 1,
            sin(theta), cos(theta), 2,
            0, 0, 1;
        Eigen::Vector3d res = mat * P;
        return res;
    }
/**
 * @brief rendering loop
 *
 */
inline void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    /*
    ################## just can draw points ##################
    */

    task1();

    /*
    ################## just can draw points ##################
    */
    glEnd();
    glFlush();
}
