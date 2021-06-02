#pragma once

#include "camera.h"
#include "renderer.h"
#include "renderer_types.h"
#include "screen.h"
#include "triangle.h"
#include "world.h"

#include "SFML/Graphics.hpp"
#include "SFML/System/Clock.hpp"

namespace application {

class Application {
 public:
  using BorderType = Camera::BorderType;

  Application();
  Application(const World &world, const Camera &camera, const Screen &screen);

  void Create(const World &world, const Camera &camera, const Screen &screen);

  void AddTriangle(const Triangle &triangle);
  void AddTriangles(const World::TriangleVec &triangles);

  void setCamera(SpaceCoordsType x1, SpaceCoordsType y1, SpaceCoordsType z1,
                 SpaceCoordsType x2, SpaceCoordsType y2, SpaceCoordsType z2);
  void setCameraPosition(SpaceCoordsType x, SpaceCoordsType y,
                         SpaceCoordsType z);
  void setCameraDirection(SpaceCoordsType x, SpaceCoordsType y,
                          SpaceCoordsType z);
  void setPivot(SpaceCoordsType x, SpaceCoordsType y, SpaceCoordsType z);
  void setFrustum(BorderType l, BorderType r, BorderType b, BorderType t,
                  BorderType n, BorderType f);

  void setScreen(Screen::SizeType w, Screen::SizeType h);

  void RunInteractiveScene(void);
  void RunRotatingScene(SpaceCoordsType radius, SpaceCoordsType speed);

  void setFPSVisible(bool is_visible);
  void setWireframeVisible(bool is_visible);
  void setWireframeColor(const ColorType &color);

 private:
  void InitWindow(void);
  void Draw(float framerate = 0);

  float getFramerate(void);

  Renderer renderer_;
  sf::RenderWindow window_;

  World world_;
  Camera camera_;
  Screen screen_;

  Screen::SizeType w_, h_;
  sf::Clock clock_;
  bool is_visible_fps_;
};

}  // namespace application
