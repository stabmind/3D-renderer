#include "application.h"
#include "screen.h"
#include "triangle.h"

#include "Eigen/Dense"

using application::Application;
using application::Screen;
using application::Triangle;

void AddCube(Application *p_app) {
  Eigen::Vector3d v1(-2, -2, 2);
  Screen::ColorType c1(0, 0, 255);

  Eigen::Vector3d v2(2, -2, 2);
  Screen::ColorType c2(255, 0, 0);

  Eigen::Vector3d v3(2, 2, 2);
  Screen::ColorType c3(255, 0, 0);

  Eigen::Vector3d v4(-2, 2, 2);
  Screen::ColorType c4(0, 0, 255);

  Eigen::Vector3d v5(-2, -2, -2);
  Screen::ColorType c5(255, 0, 255);

  Eigen::Vector3d v6(2, -2, -2);
  Screen::ColorType c6(255, 255, 0);

  Eigen::Vector3d v7(2, 2, -2);
  Screen::ColorType c7(255, 255, 0);

  Eigen::Vector3d v8(-2, 2, -2);
  Screen::ColorType c8(0, 255, 0);

  // back
  Triangle B1(v5, c5, v6, c6, v7, c7);
  Triangle B2(v5, c5, v8, c8, v7, c7);

  // front
  Triangle F1(v1, c1, v2, c2, v3, c3);
  Triangle F2(v1, c1, v4, c4, v3, c3);

  // left
  Triangle L1(v1, c1, v5, c5, v8, c8);
  Triangle L2(v1, c1, v4, c4, v8, c8);

  // right
  Triangle R1(v2, c2, v6, c6, v7, c7);
  Triangle R2(v2, c2, v3, c3, v7, c7);

  // bottom
  Triangle D1(v1, c1, v2, c2, v6, c6);
  Triangle D2(v1, c1, v5, c5, v6, c6);

  // top
  Triangle T1(v7, c7, v3, c3, v4, c4);
  Triangle T2(v7, c7, v8, c8, v4, c4);

  p_app->AddTriangles({B1, B2, F1, F2, L1, L2, R1, R2, D1, D2, T1, T2});
}

void AddPyramid(Application *p_app) {
  Eigen::Vector3d v1(-4, 2, -2);
  Screen::ColorType c1(0, 255, 0);

  Eigen::Vector3d v2(4, 2, -2);
  Screen::ColorType c2(255, 255, 0);

  Eigen::Vector3d v3(-4, 2, 2);
  Screen::ColorType c3(0, 0, 255);

  Eigen::Vector3d v4(4, 2, 2);
  Screen::ColorType c4(255, 0, 0);

  Eigen::Vector3d v5(0, 4, 0);
  Screen::ColorType c5(0, 255, 255);

  // bottom
  Triangle B1(v1, c1, v2, c2, v3, c3);
  Triangle B2(v2, c2, v3, c3, v4, c4);

  // right
  Triangle R(v2, c2, v5, c5, v4, c4);

  // left
  Triangle L(v1, c1, v5, c5, v3, c3);

  // front
  Triangle F(v3, c3, v5, c5, v4, c4);

  // back
  Triangle B(v1, c1, v5, c5, v2, c2);

  p_app->AddTriangles({B1, B2, R, L, F, B});
}

void set_world(Application *p_app) {
  AddCube(p_app);
  AddPyramid(p_app);
}

int main() {
  Application app;

  set_world(&app);
  app.setScreen(800, 600);

  app.setFPSVisible(true);

  app.RunRotatingScene(20, 0.6);
  return 0;
}
