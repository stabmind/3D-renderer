#pragma once

#include "triangle.h"

#include <vector>

namespace application {

class World {
 public:
  using TriangleVec = std::vector<Triangle>;

  World() = default;
  World(const TriangleVec &triangles);

  void AddTriangle(const Triangle &triangle);
  void AddTriangles(const World::TriangleVec &triangles);

  TriangleVec::iterator begin();
  TriangleVec::const_iterator begin() const;
  TriangleVec::iterator end();
  TriangleVec::const_iterator end() const;

 private:
  TriangleVec triangles_;
};

}  // namespace application
