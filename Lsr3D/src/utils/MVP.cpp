#include <Lsr3D/utils/MVP.h>
using namespace utils;
Eigen::Matrix4f MVP::cal_view_matrix(const Eigen::Vector3f& eye,
                                     const Eigen::Vector3f& center,
                                     const Eigen::Vector3f& up)
{
    Eigen::Vector3f f = (center - eye).normalized();
    Eigen::Vector3f s = f.cross(up).normalized();
    Eigen::Vector3f u = s.cross(f);
    Eigen::Matrix4f view;
    view << s.x(), s.y(), s.z(), -s.dot(eye),
        u.x(), u.y(), u.z(), -u.dot(eye),
        -f.x(), -f.y(), -f.z(), f.dot(eye),
        0, 0, 0, 1;
    return view;
}

Eigen::Matrix4f MVP::cal_model_matrix(const Eigen::Vector3f& translation,
                                      const Eigen::Vector3f& rotation,
                                      const Eigen::Vector3f& scale)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // 平移矩阵
    Eigen::Matrix4f trans = Eigen::Matrix4f::Identity();
    trans.block<3, 1>(0, 3) = translation;

    // 旋转矩阵
    Eigen::AngleAxisf rx(rotation.x(), Eigen::Vector3f::UnitX());
    Eigen::AngleAxisf ry(rotation.y(), Eigen::Vector3f::UnitY());
    Eigen::AngleAxisf rz(rotation.z(), Eigen::Vector3f::UnitZ());
    Eigen::Matrix3f R = rz.toRotationMatrix() * ry.toRotationMatrix() * rx.toRotationMatrix();
    Eigen::Matrix4f rot = Eigen::Matrix4f::Identity();
    rot.block<3, 3>(0, 0) = R;

    // 缩放矩阵
    Eigen::Matrix4f sca = Eigen::Matrix4f::Identity();
    sca(0, 0) = scale.x();
    sca(1, 1) = scale.y();
    sca(2, 2) = scale.z();

    // 模型矩阵 = T * R * S
    model = trans * rot * sca;

    return model;
}

Eigen::Matrix4f MVP::cal_projection_matrix(const float& fov, const float& aspect, const float& near,
                                           const float& far)
{
    Eigen::Matrix4f projection;
    float rad = fov * M_PI / 180;
    float tan_half_fov = tan(rad / 2);
    projection << 1 / (aspect * tan_half_fov), 0, 0, 0,
        0, 1 / tan_half_fov, 0, 0,
        0, 0, (far + near) / (far - near),2* (far * near) / (far - near),
        0, 0, -1, 0;
    return projection;
}
