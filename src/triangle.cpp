#include "triangle.h"

#include "Eigen/Dense"

#include <array>
#include <assert.h>

namespace application {

Triangle::Triangle(const Eigen::Vector3d &p1, ColorType c1,
                   const Eigen::Vector3d &p2, ColorType c2,
                   const Eigen::Vector3d &p3, ColorType c3)
    : hpoints_{p1.homogeneous(), p2.homogeneous(), p3.homogeneous()},
      points_colors_{c1, c2, c3} {}

// clang-format off
Triangle::Triangle(SpaceCoordsType x1, SpaceCoordsType y1, SpaceCoordsType z1,
                   SpaceCoordsType x2, SpaceCoordsType y2, SpaceCoordsType z2,
                   SpaceCoordsType x3, SpaceCoordsType y3, SpaceCoordsType z3)
    : hpoints_{Eigen::Vector4d(x1, y1, z1, 1), Eigen::Vector4d(x2, y2, z2, 1),
               Eigen::Vector4d(x3, y3, z3, 1)} {}
// clang-format on

// clang-format off
void Triangle::setColors(sf::Uint8 r1, sf::Uint8 g1, sf::Uint8 b1,
               		   sf::Uint8 r2, sf::Uint8 g2, sf::Uint8 b2,
               		   sf::Uint8 r3, sf::Uint8 g3, sf::Uint8 b3,
               		   sf::Uint8 a1, sf::Uint8 a2, sf::Uint8 a3) {
	points_colors_ =
		{ColorType(r1, g1, b1, a1), ColorType(r2, g2, b2, a2),
	   ColorType(r3, g3, b3, a3)};
}
// clang-format on

Eigen::Vector3d Triangle::getPoint(IndexType i) const {
  assert(hpoints_[i][3] != 0 && "the fourth coordinate must not be zero");
  return hpoints_[i].hnormalized();
}

Triangle::ColorType Triangle::getColor(IndexType i) const {
  return points_colors_[i];
}

std::array<Eigen::Vector4d, 3>::iterator Triangle::begin() {
  return hpoints_.begin();
}

std::array<Eigen::Vector4d, 3>::const_iterator Triangle::begin() const {
  return hpoints_.begin();
}

std::array<Eigen::Vector4d, 3>::iterator Triangle::end() {
  return hpoints_.end();
}

std::array<Eigen::Vector4d, 3>::const_iterator Triangle::end() const {
  return hpoints_.end();
}

} // namespace application
