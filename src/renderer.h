#pragma once

#include "camera.h"
#include "renderer_types.h"
#include "screen.h"
#include "triangle.h"
#include "world.h"

#include "Eigen/Dense"

#include <array>
#include <vector>

namespace application {

class Renderer {
 public:
  using ScreenCoordsType = Screen::ScreenCoordsType;
  using TriangleVec = World::TriangleVec;
  using PointVec_2d = std::vector<Eigen::Vector2d>;
  using VertexVec = std::vector<Vertex>;

  Renderer();

  void Render(const World &world, const Camera &camera,
              Screen *p_screen) noexcept;

  void setWireframeVisible(bool is_visible);
  void setWireframeColor(const ColorType &color);

 private:
  void PrepareRenderer(Screen *screen);
  void PrepareRasterization(void);

  void MakeViewTransformation(const Camera &camera, Triangle *p_triangle) const;
  void MakeProjectionTransformation(const Camera &camera,
                                    Triangle *p_triangle) const;
  void Clip(const Triangle &triangle, const PointVec_2d &poly_2d,
            TriangleVec *p_triangles) const noexcept;
  void Rasterize(const Triangle &triangle, Screen *p_screen) noexcept;

  void FrustumClip_xz(const Camera &camera, const Triangle &triangle,
                      TriangleVec *p_triangles) const;
  void FrustumClip_yz(const Camera &camera, const Triangle &triangle,
                      TriangleVec *p_triangles) const;

  std::vector<int> SelectPointsInPolygon_2d(
      const PointVec_2d &inner_poly,
      const PointVec_2d &outer_poly) const noexcept;

  void IntersectPolygons_2d(const Triangle &triangle,
                            const PointVec_2d &triangle_2d,
                            const PointVec_2d &poly_2d,
                            VertexVec *p_vertices) const noexcept;

  bool isCorrectPolygon_2d(const PointVec_2d &polygon) const;

  void RemoveEqualVertices(VertexVec *p_vertices) const;

  void Triangulate(VertexVec vertices, TriangleVec *p_triangles) const noexcept;
  void DetermineAxisForTriangulation(const VertexVec &vertices, int *p_x_axis,
                                     int *p_y_axis) const noexcept;

  // clang-format off
  void RasterizeSegment(ScreenCoordsType x1,
                        ScreenCoordsType y1,
                        SpaceCoordsType z1,
                        ColorType c1,
                        ScreenCoordsType x2,
                        ScreenCoordsType y2,
                        SpaceCoordsType z2,
                        ColorType c2,
                        Screen *p_screen,
                        bool is_edge = false) noexcept;
  // clang-format on
  void RasterizeTriangle(const Triangle &triangle, Screen *p_screen) noexcept;

  void getScreenCoords(const Vector3d &p, ScreenCoordsType *x,
                       ScreenCoordsType *y, SpaceCoordsType *z) const;
  Vector3d FitPoint(const Vector3d &p) const;

  const ColorType kBrownColor_ = ColorType(150, 75, 0);

  static constexpr SpaceCoordsType kEPS_ = 1e-9;

  Screen::SizeType w_, h_;
  std::vector<ScreenCoordsType> vmin_x_, vmax_x_;
  std::vector<SpaceCoordsType> vmin_x_z_, vmax_x_z_;
  std::vector<ColorType> vmin_x_color_, vmax_x_color_;
  ScreenCoordsType min_y_, max_y_;

  bool is_visible_wireframe_;
  ColorType wireframe_color_;
};

}  // namespace application
