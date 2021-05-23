#include "renderer.h"
#include "camera.h"
#include "screen.h"
#include "triangle.h"
#include "world.h"

#include "Eigen/Dense"

#include <algorithm>
#include <assert.h>
#include <cmath>
#include <vector>

namespace application {

Eigen::Vector2d::Scalar operator*(const Eigen::Vector2d &a,
                                  const Eigen::Vector2d &b) {
  return a[0] * b[1] - a[1] * b[0];
}

bool operator==(const Eigen::Vector3d &a, const Eigen::Vector3d &b) {
  static constexpr double EPS = 1e-12;
  return std::abs(a[0] - b[0]) < EPS && std::abs(a[1] - b[1]) < EPS &&
         std::abs(a[2] - b[2]) < EPS;
}

Renderer::Renderer()
    : is_visible_wireframe_(false), wireframe_color_(kBrownColor_) {}

void Renderer::Render(const World &world, const Camera &camera,
                      Screen *p_screen) noexcept {
  assert(p_screen != nullptr && "p_screen must not be nullptr");

  PrepareRenderer(p_screen);

  for (const auto &it : world) {
    Triangle triangle = it;
    ViewTransform(camera, &triangle);

    TriangleVec frustum_clip_xz_triangles;
    FrustumClip_xz(camera, triangle, &frustum_clip_xz_triangles);

    TriangleVec triangles;
    for (const auto &it : frustum_clip_xz_triangles) {
      FrustumClip_yz(camera, it, &triangles);
    }

    for (const auto &it : triangles) {
      Triangle triangle = it;
      ProjectionTransform(camera, &triangle);
      Rasterize(triangle, p_screen);
    }
  }
}

void Renderer::setWireframeVisible(bool is_visible) {
  is_visible_wireframe_ = is_visible;
}

void Renderer::setWireframeColor(const ColorType &color) {
  wireframe_color_ = color;
}

void Renderer::PrepareRenderer(Screen *p_screen) {
  w_ = p_screen->getW();
  h_ = p_screen->getH();
  p_screen->clear();
}

void Renderer::PrepareRasterization() {
  vmin_x_.assign(h_, w_ - 1);
  vmax_x_.assign(h_, 0);
  vmin_x_z_.resize(h_);
  vmax_x_z_.resize(h_);
  vmin_x_color_.resize(h_);
  vmax_x_color_.resize(h_);
  min_y_ = h_ - 1;
  max_y_ = 0;
}

void Renderer::ViewTransform(const Camera &camera, Triangle *p_triangle) const {
  assert(p_triangle != nullptr && "p_triangle must not be nullptr");
  Eigen::Matrix4d view_matrix = camera.getViewMatrix();
  for (auto &it : *p_triangle) {
    it = view_matrix * it;
  }
}

void Renderer::ProjectionTransform(const Camera &camera,
                                   Triangle *p_triangle) const {
  assert(p_triangle != nullptr && "p_triangle must not be nullptr");
  Eigen::Matrix4d projection_matrix = camera.getProjectionMatrix();
  for (auto &it : *p_triangle) {
    it = projection_matrix * it;
  }
}

void Renderer::Clip(const Triangle &triangle, const PointVec_2d &poly_2d,
                    TriangleVec *p_triangles) const noexcept {
  const Eigen::Vector3d &tp1 = triangle.getPoint(0);
  const Eigen::Vector3d &tp2 = triangle.getPoint(1);
  const Eigen::Vector3d &tp3 = triangle.getPoint(2);

  const ColorType &tc1 = triangle.getColor(0);
  const ColorType &tc2 = triangle.getColor(1);
  const ColorType &tc3 = triangle.getColor(2);

  PointVec_2d triangle_2d = {Eigen::Vector2d(tp1[0], tp1[1]),
                             Eigen::Vector2d(tp2[0], tp2[1]),
                             Eigen::Vector2d(tp3[0], tp3[1])};

  auto triangle_selected = SelectPointsInPolygon_2d(triangle_2d, poly_2d);
  auto poly_selected = SelectPointsInPolygon_2d(poly_2d, triangle_2d);

  // clang-format off
  auto getTriangleArea_2d =
    [](const Eigen::Vector2d &a, const Eigen::Vector2d &b,
       const Eigen::Vector2d &c) {
      return std::abs(Eigen::Vector2d(b - a) * Eigen::Vector2d(c - a));
    };
  // clang-format on

  VertexVec vertices;

  for (auto it : triangle_selected) {
    vertices.push_back({triangle.getPoint(it), triangle.getColor(it)});
  }

  for (auto it : poly_selected) {
    const auto &u = poly_2d[it];
    SpaceCoordsType A = getTriangleArea_2d(u, triangle_2d[1], triangle_2d[2]);
    SpaceCoordsType B = getTriangleArea_2d(u, triangle_2d[0], triangle_2d[2]);
    SpaceCoordsType C = getTriangleArea_2d(u, triangle_2d[0], triangle_2d[1]);

    SpaceCoordsType alpha = A / (A + B + C);
    SpaceCoordsType beta = B / (A + B + C);
    SpaceCoordsType gamma = C / (A + B + C);

    SpaceCoordsType x = u[0];
    SpaceCoordsType y = u[1];
    SpaceCoordsType z = alpha * tp1[2] + beta * tp2[2] + gamma * tp3[2];
    ColorType c(alpha * tc1.r + beta * tc2.r + gamma * tc3.r,
                alpha * tc1.g + beta * tc2.g + gamma * tc3.g,
                alpha * tc1.b + beta * tc2.b + gamma * tc3.b,
                alpha * tc1.a + beta * tc2.a + gamma * tc3.a);

    vertices.push_back({Eigen::Vector3d(x, y, z), c});
  }

  IntersectPolygons_2d(triangle, triangle_2d, poly_2d, &vertices);

  RemoveEqualVertices(&vertices);

  Triangulate(vertices, p_triangles);
}

void Renderer::Rasterize(const Triangle &triangle, Screen *p_screen) noexcept {
  PrepareRasterization();

  ScreenCoordsType x1, y1;
  SpaceCoordsType z1;
  getScreenCoords(triangle.getPoint(0), &x1, &y1, &z1);
  Triangle::ColorType c1 = triangle.getColor(0);

  ScreenCoordsType x2, y2;
  SpaceCoordsType z2;
  getScreenCoords(triangle.getPoint(1), &x2, &y2, &z2);
  Triangle::ColorType c2 = triangle.getColor(1);

  ScreenCoordsType x3, y3;
  SpaceCoordsType z3;
  getScreenCoords(triangle.getPoint(2), &x3, &y3, &z3);
  Triangle::ColorType c3 = triangle.getColor(2);

  RasterizeSegment(x1, y1, z1, c1, x2, y2, z2, c2, p_screen, true);
  RasterizeSegment(x1, y1, z1, c1, x3, y3, z3, c3, p_screen, true);
  RasterizeSegment(x2, y2, z2, c2, x3, y3, z3, c3, p_screen, true);

  RasterizeTriangle(triangle, p_screen);
}

void Renderer::FrustumClip_xz(const Camera &camera, const Triangle &triangle,
                              TriangleVec *p_triangles) const {
  Camera::BorderType l = camera.getL();
  Camera::BorderType r = camera.getR();
  Camera::BorderType n = camera.getN();
  Camera::BorderType f = camera.getF();

  assert(n != 0 && "n must not be 0");

  Camera::BorderType k = f / n;

  const PointVec_2d frustum_xz = {
      Eigen::Vector2d(l, -n), Eigen::Vector2d(r, -n),
      Eigen::Vector2d(r * k, -f), Eigen::Vector2d(l * k, -f)};

  auto ReversePoint_xzy = [](const Eigen::Vector3d &p) {
    return Eigen::Vector3d(p[0], p[2], p[1]);
  };

  const Eigen::Vector3d &tp1 = triangle.getPoint(0);
  const Eigen::Vector3d &tp2 = triangle.getPoint(1);
  const Eigen::Vector3d &tp3 = triangle.getPoint(2);

  const ColorType &tc1 = triangle.getColor(0);
  const ColorType &tc2 = triangle.getColor(1);
  const ColorType &tc3 = triangle.getColor(2);

  // clang-format off
  const Triangle triangle_xzy(ReversePoint_xzy(tp1), tc1,
                              ReversePoint_xzy(tp2), tc2,
                              ReversePoint_xzy(tp3), tc3);
  // clang-format on

  TriangleVec triangles_xzy;
  Clip(triangle_xzy, frustum_xz, &triangles_xzy);

  for (const auto &it : triangles_xzy) {
    const Eigen::Vector3d &tp1 = it.getPoint(0);
    const Eigen::Vector3d &tp2 = it.getPoint(1);
    const Eigen::Vector3d &tp3 = it.getPoint(2);

    const ColorType &tc1 = it.getColor(0);
    const ColorType &tc2 = it.getColor(1);
    const ColorType &tc3 = it.getColor(2);

    // clang-format off
    p_triangles->push_back({ReversePoint_xzy(tp1), tc1,
                            ReversePoint_xzy(tp2), tc2,
                            ReversePoint_xzy(tp3), tc3});
    // clang-format on
  }
}

void Renderer::FrustumClip_yz(const Camera &camera, const Triangle &triangle,
                              TriangleVec *p_triangles) const {
  Camera::BorderType b = camera.getB();
  Camera::BorderType t = camera.getT();
  Camera::BorderType n = camera.getN();
  Camera::BorderType f = camera.getF();

  assert(n != 0 && "n must not be 0");

  Camera::BorderType k = f / n;

  const PointVec_2d frustum_yz = {
      Eigen::Vector2d(b, -n), Eigen::Vector2d(t, -n),
      Eigen::Vector2d(t * k, -f), Eigen::Vector2d(b * k, -f)};

  auto ReversePoint_yzx = [](const Eigen::Vector3d &p) {
    return Eigen::Vector3d(p[1], p[2], p[0]);
  };

  auto UnreversePoint_yzx = [](const Eigen::Vector3d &p) {
    return Eigen::Vector3d(p[2], p[0], p[1]);
  };

  const Eigen::Vector3d &tp1 = triangle.getPoint(0);
  const Eigen::Vector3d &tp2 = triangle.getPoint(1);
  const Eigen::Vector3d &tp3 = triangle.getPoint(2);

  const ColorType &tc1 = triangle.getColor(0);
  const ColorType &tc2 = triangle.getColor(1);
  const ColorType &tc3 = triangle.getColor(2);

  // clang-format off
  const Triangle triangle_yzx(ReversePoint_yzx(tp1), tc1,
                              ReversePoint_yzx(tp2), tc2,
                              ReversePoint_yzx(tp3), tc3);
  // clang-format on

  TriangleVec triangles_yzx;
  Clip(triangle_yzx, frustum_yz, &triangles_yzx);

  for (const auto &it : triangles_yzx) {
    const Eigen::Vector3d &tp1 = it.getPoint(0);
    const Eigen::Vector3d &tp2 = it.getPoint(1);
    const Eigen::Vector3d &tp3 = it.getPoint(2);

    const ColorType &tc1 = it.getColor(0);
    const ColorType &tc2 = it.getColor(1);
    const ColorType &tc3 = it.getColor(2);

    // clang-format off
    p_triangles->push_back({UnreversePoint_yzx(tp1), tc1,
                            UnreversePoint_yzx(tp2), tc2,
                            UnreversePoint_yzx(tp3), tc3});
    // clang-format on
  }
}

void Renderer::SquareClip(const Triangle &triangle, TriangleVec *p_triangles) {
  // clang-format off
  const PointVec_2d square_2d = {
      Eigen::Vector2d(-1, -1), Eigen::Vector2d(1, -1),
      Eigen::Vector2d(1, 1), Eigen::Vector2d(-1, 1)};
  // clang-format on
  Clip(triangle, square_2d, p_triangles);
}

std::vector<int> Renderer::SelectPointsInPolygon_2d(
    const PointVec_2d &inner_poly,
    const PointVec_2d &outer_poly) const noexcept {
  auto getSign = [](SpaceCoordsType x) {
    if (-kEPS_ < x && x < kEPS_) {
      return 0;
    }
    return x < 0 ? -1 : 1;
  };

  std::vector<int> selected;

  if (isCorrectPolygon_2d(outer_poly)) {
    size_t n = inner_poly.size();
    size_t m = outer_poly.size();
    for (size_t i = 0; i < n; ++i) {
      bool is_selected = true;
      for (size_t j = 0; j < m; ++j) {
        Eigen::Vector2d u = outer_poly[(j + 1) % m] - outer_poly[j];
        Eigen::Vector2d v1 = outer_poly[(j + 2) % m] - outer_poly[j];
        Eigen::Vector2d v2 = inner_poly[i] - outer_poly[j];
        if (std::abs(getSign(u * v1) - getSign(u * v2)) == 2) {
          is_selected = false;
          break;
        }
      }
      if (is_selected) {
        selected.push_back(i);
      }
    }
  }

  return selected;
}

void Renderer::RemoveEqualVertices(VertexVec *p_vertices) const {
  VertexVec new_vec;
  for (const auto &it : *p_vertices) {
    bool is_uniq = true;
    for (const auto &jt : new_vec) {
      if (it.point == jt.point) {
        is_uniq = false;
      }
    }
    if (is_uniq) {
      new_vec.push_back(it);
    }
  }
  *p_vertices = new_vec;
}

void Renderer::IntersectPolygons_2d(const Triangle &triangle,
                                    const PointVec_2d &triangle_2d,
                                    const PointVec_2d &poly_2d,
                                    VertexVec *p_vertices) const noexcept {
  auto intersectSegments =
      [](const Eigen::Vector2d &v1, const Eigen::Vector2d &v2,
         const Eigen::Vector2d &u1, const Eigen::Vector2d &u2,
         Eigen::Vector2d *p_point) {
        Eigen::Vector2d v = v2 - v1;
        Eigen::Vector2d u = u2 - u1;
        Eigen::Vector2d n(-v[1], v[0]);
        auto dp = u.dot(n);
        if (-kEPS_ < dp && dp < kEPS_) {
          return false;
        }
        SpaceCoordsType t = (v1.dot(n) - u1.dot(n)) / u.dot(n);
        *p_point = u1 + u * t;
        return -kEPS_ < t && t < 1 + kEPS_;
      };

  // clang-format off
  auto isOnSegment =
    [](const Eigen::Vector2d &p, const Eigen::Vector2d v1,
       const Eigen::Vector2d v2) {
      return (p - v1).dot(p - v2) < 0;
    };
  // clang-format on

  size_t n = triangle_2d.size();
  size_t m = poly_2d.size();
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < m; ++j) {
      const Eigen::Vector3d &tp1 = triangle.getPoint(i);
      const Eigen::Vector3d &tp2 = triangle.getPoint((i + 1) % n);
      const ColorType &tc1 = triangle.getColor(i);
      const ColorType &tc2 = triangle.getColor((i + 1) % n);

      const Eigen::Vector2d &v1 = triangle_2d[i];
      const Eigen::Vector2d &v2 = triangle_2d[(i + 1) % n];
      const Eigen::Vector2d &u1 = poly_2d[j];
      const Eigen::Vector2d &u2 = poly_2d[(j + 1) % m];

      Eigen::Vector2d intersection_point;
      if (intersectSegments(v1, v2, u1, u2, &intersection_point) &&
          isOnSegment(intersection_point, v1, v2)) {
        SpaceCoordsType alpha =
            (intersection_point - v2).norm() / (v2 - v1).norm();
        SpaceCoordsType beta =
            (intersection_point - v1).norm() / (v2 - v1).norm();

        SpaceCoordsType x = intersection_point[0];
        SpaceCoordsType y = intersection_point[1];
        SpaceCoordsType z = alpha * tp1[2] + beta * tp2[2];
        // clang-format off
        ColorType c(alpha * tc1.r + beta * tc2.r,
                    alpha * tc1.g + beta * tc2.g,
                    alpha * tc1.b + beta * tc2.b,
                    alpha * tc1.a + beta * tc2.a);
        // clang-format on

        p_vertices->push_back({Eigen::Vector3d(x, y, z), c});
      }
    }
  }
}

bool Renderer::isCorrectPolygon_2d(const PointVec_2d &polygon) const {
  for (size_t i = 1; i + 1 < polygon.size(); ++i) {
    SpaceCoordsType cross_product =
        Eigen::Vector2d(polygon[i] - polygon[0]) *
        Eigen::Vector2d(polygon[i + 1] - polygon[0]);
    if (cross_product < -kEPS_ || kEPS_ < cross_product) {
      return true;
    }
  }
  return false;
}

void Renderer::Triangulate(VertexVec vertices,
                           TriangleVec *p_triangles) const noexcept {
  if (vertices.size() < 3) {
    return;
  }

  int x_axis = -1, y_axis = -1;
  DetermineAxisForTriangulation(vertices, &x_axis, &y_axis);

  assert(x_axis != -1 && y_axis != -1 && "axes must be determined");

  auto tmp_vert = vertices[0];

  Eigen::Vector3d start_v = vertices[0].point;
  for (const auto &it : vertices) {
    if ((it.point[x_axis] < start_v[x_axis]) ||
        (it.point[x_axis] == start_v[x_axis] &&
         it.point[y_axis] < start_v[y_axis])) {
      start_v = it.point;
      tmp_vert = it;
    }
  }

  VertexVec tmp;
  for (const auto &it : vertices) {
    if (it.point != start_v) {
      tmp.push_back(it);
    }
  }

  // clang-format off
  auto cmp = [start_v, x_axis, y_axis](const Vertex &a, const Vertex &b) {
    Eigen::Vector2d start_v_2d(start_v[x_axis], start_v[y_axis]);
    Eigen::Vector2d a_2d(a.point[x_axis], a.point[y_axis]);
    Eigen::Vector2d b_2d(b.point[x_axis], b.point[y_axis]);
    return Eigen::Vector2d(a_2d - start_v_2d) *
           Eigen::Vector2d(b_2d - start_v_2d) > 0;
  };
  // clang-format on

  sort(tmp.begin(), tmp.end(), cmp);

  vertices = tmp;
  vertices.push_back(tmp_vert);

  size_t n = vertices.size();
  for (size_t i = 1; i + 1 < n; ++i) {
    const Eigen::Vector3d &p1 = vertices[0].point;
    const Eigen::Vector3d &p2 = vertices[i].point;
    const Eigen::Vector3d &p3 = vertices[i + 1].point;
    const ColorType &c1 = vertices[0].color;
    const ColorType &c2 = vertices[i].color;
    const ColorType &c3 = vertices[i + 1].color;
    p_triangles->push_back(Triangle(p1, c1, p2, c2, p3, c3));
  }
}

void Renderer::DetermineAxisForTriangulation(const VertexVec &vertices,
                                             int *p_x_axis,
                                             int *p_y_axis) const noexcept {
  const size_t dim = 3;
  size_t n = vertices.size();
  for (size_t i = 0; i < dim; ++i) {
    for (size_t j = i + 1; j < dim; ++j) {
      for (size_t k = 1; k + 1 < n; ++k) {
        const Eigen::Vector3d &v = vertices[0].point - vertices[k].point;
        const Eigen::Vector3d &u = vertices[0].point - vertices[k + 1].point;
        const Eigen::Vector2d v_2d(v[i], v[j]);
        const Eigen::Vector2d u_2d(u[i], u[j]);
        auto cross_product = v_2d * u_2d;
        if (cross_product < -kEPS_ || kEPS_ < cross_product) {
          *p_x_axis = i;
          *p_y_axis = j;
          return;
        }
      }
    }
  }
}

// clang-format off
void Renderer::RasterizeSegment(ScreenCoordsType x1,
                                ScreenCoordsType y1,
                                SpaceCoordsType z1,
                                ColorType c1,
                                ScreenCoordsType x2,
                                ScreenCoordsType y2,
                                SpaceCoordsType z2,
                                ColorType c2,
                                Screen *p_screen,
                                bool is_edge) noexcept {
  // clang-format on
  auto dx = x2 - x1;
  auto dy = y2 - y1;
  auto dz = z2 - z1;
  int dr = (int)c2.r - c1.r;
  int dg = (int)c2.g - c1.g;
  int db = (int)c2.b - c1.b;
  int da = (int)c2.a - c1.a;

  bool is_swaped = false;
  if (dx == 0 || std::abs((SpaceCoordsType)dy / dx) > 1) {
    std::swap(x1, y1);
    std::swap(x2, y2);
    std::swap(dx, dy);
    is_swaped = true;
  }

  if (x1 > x2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
    std::swap(z1, z2);
    std::swap(c1, c2);
    dx *= -1;
    dy *= -1;
    dz *= -1;
    dr *= -1;
    dg *= -1;
    db *= -1;
    da *= -1;
  }

  auto isVisible = [p_screen](SpaceCoordsType x, SpaceCoordsType y,
                              SpaceCoordsType z) {
    return -1 - kEPS_ < z && z < p_screen->getZ(y, x) + kEPS_;
  };

  auto isInCube = [p_screen](SpaceCoordsType x, SpaceCoordsType y,
                             SpaceCoordsType z) {
    return -1 - kEPS_ < z && z < 1 + kEPS_;
  };

  for (ScreenCoordsType i = x1; i <= x2; ++i) {
    SpaceCoordsType alpha = (i == x1 ? 0 : (SpaceCoordsType)(i - x1) / dx);
    ScreenCoordsType x = i;
    ScreenCoordsType y = y1 + 0.5 + alpha * dy;
    SpaceCoordsType z = z1 + alpha * dz;
    // clang-format off
    ColorType c(c1.r + 0.5 + alpha * dr,
                c1.g + 0.5 + alpha * dg,
                c1.b + 0.5 + alpha * db,
                c1.a + 0.5 + alpha * da);
    // clang-format on

    if (is_swaped) {
      std::swap(x, y);
    }

    if (is_edge) {
      if (x < vmin_x_[y]) {
        vmin_x_[y] = x;
        vmin_x_z_[y] = z;
        vmin_x_color_[y] = c;
      }
      if (x > vmax_x_[y]) {
        vmax_x_[y] = x;
        vmax_x_z_[y] = z;
        vmax_x_color_[y] = c;
      }

      min_y_ = std::min(y, min_y_);
      max_y_ = std::max(y, max_y_);

      if (is_visible_wireframe_ && isInCube(x, y, z)) {
        p_screen->setPixel(y, x, z, kBrownColor_.r, kBrownColor_.g,
                           kBrownColor_.b, kBrownColor_.a);
        p_screen->BlockPixel(y, x);
      }
    } else if (isVisible(x, y, z) && !p_screen->isBlockedPixel(y, x)) {
      p_screen->setPixel(y, x, z, c.r, c.g, c.b, c.a);
    }
  }
}

void Renderer::RasterizeTriangle(const Triangle &triangle,
                                 Screen *p_screen) noexcept {
  for (ScreenCoordsType y = min_y_; y <= max_y_; ++y) {
    ScreenCoordsType x1 = vmin_x_[y];
    SpaceCoordsType z1 = vmin_x_z_[y];
    ColorType c1 = vmin_x_color_[y];

    ScreenCoordsType x2 = vmax_x_[y];
    SpaceCoordsType z2 = vmax_x_z_[y];
    ColorType c2 = vmax_x_color_[y];

    RasterizeSegment(x1, y, z1, c1, x2, y, z2, c2, p_screen);
  }
}

void Renderer::getScreenCoords(const Eigen::Vector3d &p, ScreenCoordsType *x,
                               ScreenCoordsType *y, SpaceCoordsType *z) const {
  Eigen::Vector3d fit_p = FitPoint(p);
  *x = std::min((ScreenCoordsType)fit_p[0], (ScreenCoordsType)w_ - 1);
  *y = std::min((ScreenCoordsType)fit_p[1], (ScreenCoordsType)h_ - 1);
  *z = p[2];
}

Eigen::Vector3d Renderer::FitPoint(const Eigen::Vector3d &p) const {
  Eigen::Vector3d::Scalar fit_x = (p[0] + 1) / 2 * w_;
  Eigen::Vector3d::Scalar fit_y = (1 - (p[1] + 1) / 2) * h_;
  return Eigen::Vector3d(fit_x, fit_y, p[2]);
}

} // namespace application
