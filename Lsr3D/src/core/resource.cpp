/**
 * @file resource.cpp
 * @author zhywyt (zhywyt@yeah.net)
 * @brief 
 * @version 0.1
 * @date 2025-09-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <Lsr3D/core/resource.h>


namespace lsr3d
{
    Vertex operator*(const Eigen::Matrix4f& mat, const Vertex& v) {
        Eigen::Vector4f res = mat * Eigen::Vector4f(v.position[0], v.position[1], v.position[2], 1.0f);
        return Vertex(res.head<3>() / res[3]);
    }
    Normal operator*(const Eigen::Matrix4f& mat, const Normal& n) {
        Eigen::Vector4f res = mat * Eigen::Vector4f(n.normal[0], n.normal[1], n.normal[2], 0.0f);
        return Normal(res.head<3>());
    }
}
