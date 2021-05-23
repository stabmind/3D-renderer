#include "camera.h"

#include "Eigen/Dense"

#include <cmath>
#include <tuple>

namespace application {

Camera::Camera(SpaceCoordsType x1, SpaceCoordsType y1, SpaceCoordsType z1,
               SpaceCoordsType x2, SpaceCoordsType y2, SpaceCoordsType z2) {
  setCamera(x1, y1, z1, x2, y2, z2);
}

void Camera::setCamera(SpaceCoordsType x1, SpaceCoordsType y1,
                       SpaceCoordsType z1, SpaceCoordsType x2,
                       SpaceCoordsType y2, SpaceCoordsType z2) {
  setPosition(x1, y1, z1);
  setDirection(x2, y2, z2);
}

void Camera::setPosition(SpaceCoordsType x, SpaceCoordsType y,
                         SpaceCoordsType z) {
  camera_position_ = {x, y, z};
}

void Camera::setDirection(SpaceCoordsType x, SpaceCoordsType y,
                          SpaceCoordsType z) {
  camera_direction_ = {x, y, z};
}

void Camera::setPivot(SpaceCoordsType x, SpaceCoordsType y, SpaceCoordsType z) {
  pivot_ = Eigen::Vector3d(x, y, z);
  e1_ = camera_direction_.cross(pivot_).normalized();
  e2_ = -camera_direction_.cross(e1_).normalized();
  e3_ = -camera_direction_.normalized();
  CreateViewMatrix();
}

void Camera::setFrustum(BorderType l, BorderType r, BorderType b, BorderType t,
                        BorderType n, BorderType f) {
  std::tie(l_, r_, b_, t_, n_, f_) =
      std::tuple<BorderType, BorderType, BorderType, BorderType, BorderType,
                 BorderType>{l, r, b, t, n, f};
  CreateProjectionMatrix();
}

void Camera::Rotate(const Eigen::Vector3d &v, AngleType angle) {
  Eigen::Vector3d u = v.normalized();
  Eigen::Matrix3d m;

  AngleType c = std::cos(angle);
  AngleType s = std::sin(angle);

  // clang-format off
  m <<
    c + (1 - c) * u[0] * u[0], (1 - c) * u[0] * u[1] - s * u[2], (1 - c) * u[0] * u[2] + s * u[1],
    (1 - c) * u[0] * u[1] + s * u[2], c + (1 - c) * u[1] * u[1], (1 - c) * u[1] * u[2] - s * u[0],
    (1 - c) * u[0] * u[2] - s * u[1], (1 - c) * u[1] * u[2] + s * u[0], c + (1 - c) * u[2] * u[2];
  // clang-format on

  camera_direction_ = m * camera_direction_;
  pivot_ = m * pivot_;
  e1_ = m * e1_;
  e2_ = m * e2_;
  e3_ = m * e3_;

  CreateViewMatrix();
}

void Camera::Shift(const Eigen::Vector3d &h) {
  camera_position_ += h;
  CreateViewMatrix();
}

void Camera::Roll(AngleType angle) { Rotate(camera_direction_, angle); }

void Camera::Pitch(AngleType angle) { Rotate(e1_, angle); }

void Camera::Yaw(AngleType angle) { Rotate(e2_, angle); }

void Camera::MoveForward(SpaceCoordsType length) {
  Shift(camera_direction_.normalized() * length);
}

void Camera::MoveBackward(SpaceCoordsType length) {
  Shift(-camera_direction_.normalized() * length);
}

void Camera::MoveRightward(SpaceCoordsType length) {
  Shift(e1_.normalized() * length);
}

void Camera::MoveLeftward(SpaceCoordsType length) {
  Shift(-e1_.normalized() * length);
}

void Camera::MoveUpward(SpaceCoordsType length) {
  Shift(e2_.normalized() * length);
}

void Camera::MoveDownward(SpaceCoordsType length) {
  Shift(-e2_.normalized() * length);
}

Eigen::Matrix4d Camera::getViewMatrix() const { return view_matrix_; }
Eigen::Matrix4d Camera::getProjectionMatrix() const {
  return projection_matrix_;
}

Camera::BorderType Camera::getL() const { return l_; }

Camera::BorderType Camera::getR() const { return r_; }

Camera::BorderType Camera::getB() const { return b_; }

Camera::BorderType Camera::getT() const { return t_; }

Camera::BorderType Camera::getN(void) const { return n_; }

Camera::BorderType Camera::getF(void) const { return f_; }

void Camera::CreateViewMatrix() {
  Eigen::Matrix3d C;
  C << e1_, e2_, e3_;

  Eigen::Matrix4d A = Eigen::Matrix4d::Identity();
  A.block<3, 3>(0, 0) = C.transpose();

  Eigen::Matrix4d B = Eigen::Matrix4d::Identity();
  B.block<3, 1>(0, 3) = -camera_position_;

  view_matrix_ = A * B;
}

void Camera::CreateProjectionMatrix() {
  // clang-format off
  projection_matrix_ <<
    2 * n_ / (r_ - l_), 0, (r_ + l_) / (r_ - l_), 0,
    0, 2 * n_ / (t_ - b_), (t_ + b_) / (t_ - b_), 0,
    0, 0, -(f_ + n_) / (f_ - n_), -2 * n_ * f_ / (f_ - n_),
    0, 0, -1, 0;
  // clang-format on
}

} // namespace application
