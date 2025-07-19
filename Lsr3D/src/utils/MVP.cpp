#include <Lsr3D/utils/MVP.h>
using namespace utils;
Eigen::Matrix4d MVP::cal_view_matrix(const Eigen::Vector3d& eye,
                                     const Eigen::Vector3d& center,
                                     const Eigen::Vector3d& up)
{
    Eigen::Vector3d f = (center - eye).normalized();
    Eigen::Vector3d s = f.cross(up).normalized();
    Eigen::Vector3d u = s.cross(f);
    Eigen::Matrix4d view;
    view << s.x(), s.y(), s.z(), -s.dot(eye),
        u.x(), u.y(), u.z(), -u.dot(eye),
        -f.x(), -f.y(), -f.z(), f.dot(eye),
        0, 0, 0, 1;
    return view;
}

Eigen::Matrix4d MVP::cal_model_matrix(const Eigen::Vector3d& translation,
                                      const Eigen::Vector3d& rotation,
                                      const Eigen::Vector3d& scale)
{
    Eigen::Matrix4d model = Eigen::Matrix4d::Identity();

    // 平移矩阵
    Eigen::Matrix4d trans = Eigen::Matrix4d::Identity();
    trans.block<3, 1>(0, 3) = translation;

    // 旋转矩阵
    Eigen::AngleAxisd rx(rotation.x(), Eigen::Vector3d::UnitX());
    Eigen::AngleAxisd ry(rotation.y(), Eigen::Vector3d::UnitY());
    Eigen::AngleAxisd rz(rotation.z(), Eigen::Vector3d::UnitZ());
    Eigen::Matrix3d R = rz.toRotationMatrix() * ry.toRotationMatrix() * rx.toRotationMatrix();
    Eigen::Matrix4d rot = Eigen::Matrix4d::Identity();
    rot.block<3, 3>(0, 0) = R;

    // 缩放矩阵
    Eigen::Matrix4d sca = Eigen::Matrix4d::Identity();
    sca(0, 0) = scale.x();
    sca(1, 1) = scale.y();
    sca(2, 2) = scale.z();

    // 模型矩阵 = T * R * S
    model = trans * rot * sca;

    return model;
}

Eigen::Matrix4d MVP::cal_projection_matrix(const double& fov, const double& aspect, const double& near,
                                           const double& far)
{
    Eigen::Matrix4d projection;
    double rad = fov * M_PI / 180;
    double tan_half_fov = tan(rad / 2);
    projection << 1 / (aspect * tan_half_fov), 0, 0, 0,
        0, 1 / tan_half_fov, 0, 0,
        0, 0, (far + near) / (far - near),2* (far * near) / (far - near),
        0, 0, -1, 0;
    return projection;
}
