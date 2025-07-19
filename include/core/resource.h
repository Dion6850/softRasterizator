/**
 * @file resource.h
 * @author dion (hduer_zdy@outlook.com)
 * @brief vital struct
 * @version 0.1
 * @date 2025/7/13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef RESOURCE_H
#define RESOURCE_H


#include <Eigen/Core>

struct Vertex {
 // 位置信息 (必需)
 Eigen::Vector4f position;     // 齐次坐标 (x, y, z, w)

 // 颜色信息
 Eigen::Vector4f color;        // RGBA颜色

 // 纹理坐标
 Eigen::Vector2f texCoord;     // UV坐标

 // 法线信息
 Eigen::Vector3f normal;       // 法线方向

 // 切线空间信息（用于法线贴图）
 Eigen::Vector3f tangent;      // 切线
 Eigen::Vector3f bitangent;    // 副切线

 // 构造函数
 Vertex() {
  position.setZero();
  color.setOnes();          // 默认白色
  texCoord.setZero();
  normal.setZero();
  tangent.setZero();
  bitangent.setZero();
 }
};
#endif //RESOURCE_H
