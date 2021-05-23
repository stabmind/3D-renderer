#pragma once

#include "renderer_types.h"

#include "Eigen/Dense"
#include "SFML/Graphics/Color.hpp"

#include <array>

namespace application {

class Triangle {
public:
  using SpaceCoordsType = renderer_types::SpaceCoordsType;
  using ColorType = renderer_types::ColorType;
  using IndexType = int;

  // clang-format off
  Triangle(const Eigen::Vector3d &p1, ColorType c1,
           const Eigen::Vector3d &p2, ColorType c2,
           const Eigen::Vector3d &p3, ColorType c3);
  // clang-format on

  Triangle(SpaceCoordsType x1, SpaceCoordsType y1, SpaceCoordsType z1,
           SpaceCoordsType x2, SpaceCoordsType y2, SpaceCoordsType z2,
           SpaceCoordsType x3, SpaceCoordsType y3, SpaceCoordsType z);

  // clang-format off
  void setColors(sf::Uint8 r1, sf::Uint8 g1, sf::Uint8 b1,
                 sf::Uint8 r2, sf::Uint8 g2, sf::Uint8 b2,
                 sf::Uint8 r3, sf::Uint8 g3, sf::Uint8 b3,
                 sf::Uint8 a1 = 255, sf::Uint8 a2 = 255,
                 sf::Uint8 a3 = 255);
  // clang-format on

  Eigen::Vector3d getPoint(IndexType i) const;
  ColorType getColor(IndexType i) const;

  std::array<Eigen::Vector4d, 3>::iterator begin();
  std::array<Eigen::Vector4d, 3>::const_iterator begin() const;
  std::array<Eigen::Vector4d, 3>::iterator end();
  std::array<Eigen::Vector4d, 3>::const_iterator end() const;

private:
  std::array<Eigen::Vector4d, 3> hpoints_;
  std::array<ColorType, 3> points_colors_;
};

} // namespace application
