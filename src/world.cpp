#include "world.h"
#include "triangle.h"

namespace application {

World::World(const TriangleVec &triangles) : triangles_(triangles) {}

void World::AddTriangle(const Triangle &triangle) {
  triangles_.push_back(triangle);
}

void World::AddTriangles(const World::TriangleVec &triangles) {
  triangles_.reserve(triangles_.size() + triangles.size());
  for (const auto &it : triangles) {
    AddTriangle(it);
  }
}

World::TriangleVec::iterator World::begin() { return triangles_.begin(); }

World::TriangleVec::const_iterator World::begin() const {
  return triangles_.begin();
}

World::TriangleVec::iterator World::end() { return triangles_.end(); }

World::TriangleVec::const_iterator World::end() const {
  return triangles_.end();
}

}  // namespace application
