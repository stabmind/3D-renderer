#pragma once

#include "renderer_types.h"

#include "Eigen/Dense"

namespace application {

class Camera {
 public:
  using AngleType = double;
  using BorderType = SpaceCoordsType;

  Camera() = default;
  Camera(SpaceCoordsType x1, SpaceCoordsType y1, SpaceCoordsType z1,
         SpaceCoordsType x2, SpaceCoordsType y2, SpaceCoordsType z2);

  void setCamera(SpaceCoordsType x1, SpaceCoordsType y1, SpaceCoordsType z1,
                 SpaceCoordsType x2, SpaceCoordsType y2, SpaceCoordsType z2);
  void setPosition(SpaceCoordsType x, SpaceCoordsType y, SpaceCoordsType z);
  void setDirection(SpaceCoordsType x, SpaceCoordsType y, SpaceCoordsType z);
  void setPivot(SpaceCoordsType x, SpaceCoordsType y, SpaceCoordsType z);
  void setFrustum(BorderType l, BorderType r, BorderType b, BorderType t,
                  BorderType n, BorderType f);

  void Rotate(const Vector3d &v, AngleType angle);
  void Shift(const Vector3d &h);

  void Roll(AngleType angle);
  void Pitch(AngleType angle);
  void Yaw(AngleType angle);

  void MoveForward(SpaceCoordsType length);
  void MoveBackward(SpaceCoordsType length);
  void MoveRightward(SpaceCoordsType length);
  void MoveLeftward(SpaceCoordsType length);
  void MoveUpward(SpaceCoordsType length);
  void MoveDownward(SpaceCoordsType length);

  Eigen::Matrix4d getViewMatrix(void) const;
  Eigen::Matrix4d getProjectionMatrix(void) const;

  BorderType getL(void) const;
  BorderType getR(void) const;
  BorderType getB(void) const;
  BorderType getT(void) const;
  BorderType getN(void) const;
  BorderType getF(void) const;

 private:
  void CreateViewMatrix(void);
  void CreateProjectionMatrix(void);

  Vector3d camera_position_;
  Vector3d camera_direction_;
  BorderType l_, r_, b_, t_, n_, f_;
  Vector3d pivot_;

  Eigen::Matrix4d view_matrix_;
  Eigen::Matrix4d projection_matrix_;

  Vector3d e1_, e2_, e3_;
};

}  // namespace application
