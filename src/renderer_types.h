#pragma once

#include "Eigen/Dense"
#include "SFML/Graphics/Color.hpp"

namespace application {

using SpaceCoordsType = double;
using ColorType = sf::Color;

using Vector3d = Eigen::Vector3d;
using Vector4d = Eigen::Vector4d;

struct Vertex {
  Eigen::Vector3d point;
  ColorType color;
};

}  // namespace application
