#include "application.h"

using namespace application;

void add_plane(Application *p_app) {
  Vertex v1({-30, -30, 0}, {80, 80, 80});
  Vertex v2({30, -30, 0}, {80, 80, 80});
  Vertex v3({30, 30, 0}, {80, 80, 80});
  Vertex v4({-30, 30, 0}, {80, 80, 80});
  Vertex v5({-30, 0, 0}, {80, 80, 80});
  Vertex v6({0, -30, 0}, {80, 80, 80});
  Vertex v7({30, 0, 0}, {80, 80, 80});
  Vertex v8({0, 30, 0}, {80, 80, 80});
  Vertex v9({0, 0, 0}, {0, 0, 0});

  Triangle T1(v1, v5, v6);
  Triangle T2(v5, v6, v9);
  Triangle T3(v2, v6, v7);
  Triangle T4(v6, v7, v9);
  Triangle T5(v3, v7, v8);
  Triangle T6(v7, v8, v9);
  Triangle T7(v4, v5, v8);
  Triangle T8(v5, v8, v9);

  p_app->AddTriangles({T1, T2, T3, T4, T5, T6, T7, T8});
}

void add_left_mountains(Application *p_app) {
  Vertex v1({-50, -30, 0}, {80, 80, 80});
  Vertex v2({-30, -30, 0}, {80, 80, 80});
  Vertex v3({-45, -25, 40}, {255, 0, 0});
  Vertex v4({-30, -20, 0}, {80, 80, 80});
  Vertex v5({-50, -15, 0}, {80, 80, 80});
  Vertex v6({-35, -10, 60}, {0, 255, 0});
  Vertex v7({-50, 0, 0}, {80, 80, 80});
  Vertex v8({-30, 0, 0}, {80, 80, 80});
  Vertex v9({-40, 20, 30}, {0, 0, 255});
  Vertex v10({-50, 30, 0}, {80, 80, 80});
  Vertex v11({-30, 30, 0}, {80, 80, 80});

  Triangle T1(v1, v2, v3);
  Triangle T2(v2, v3, v4);
  Triangle T3(v1, v3, v5);
  Triangle T4(v3, v4, v5);
  Triangle T5(v1, v2, v5);
  Triangle T6(v2, v4, v5);

  p_app->AddTriangles({T1, T2, T3, T4, T5, T6});

  Triangle T7(v4, v5, v6);
  Triangle T8(v4, v6, v8);
  Triangle T9(v6, v7, v8);
  Triangle T10(v5, v6, v7);
  Triangle T11(v4, v5, v7);
  Triangle T12(v4, v7, v8);

  p_app->AddTriangles({T7, T8, T9, T10, T11, T12});

  Triangle T13(v7, v8, v9);
  Triangle T14(v8, v9, v11);
  Triangle T15(v9, v10, v11);
  Triangle T16(v7, v9, v10);
  Triangle T17(v7, v8, v10);
  Triangle T18(v8, v10, v11);

  p_app->AddTriangles({T13, T14, T15, T16, T17, T18});
}

void add_right_mountains(Application *p_app) {
  Vertex v1({30, -30, 0}, {80, 80, 80});
  Vertex v2({50, -30, 0}, {80, 80, 80});
  Vertex v3({45, -25, 100}, {255, 255, 0});
  Vertex v4({50, -15, 0}, {80, 80, 80});
  Vertex v5({35, -5, 70}, {0, 255, 255});
  Vertex v6({30, 10, 0}, {80, 80, 80});
  Vertex v7({45, 10, 80}, {255, 0, 255});
  Vertex v8({35, 25, 30}, {20, 80, 20});
  Vertex v9({30, 30, 0}, {80, 80, 80});
  Vertex v10({50, 30, 0}, {80, 80, 80});

  Triangle T1(v1, v2, v3);
  Triangle T2(v2, v3, v4);
  Triangle T3(v1, v3, v4);
  Triangle T4(v1, v2, v4);

  p_app->AddTriangles({T1, T2, T3, T4});

  Triangle T5(v1, v4, v5);
  Triangle T6(v4, v5, v6);
  Triangle T7(v1, v5, v6);
  Triangle T8(v1, v4, v6);

  p_app->AddTriangles({T5, T6, T7, T8});

  Triangle T9(v4, v6, v7);
  Triangle T10(v4, v7, v10);
  Triangle T11(v6, v7, v10);
  Triangle T12(v4, v6, v10);

  p_app->AddTriangles({T9, T10, T11, T12});

  Triangle T13(v6, v8, v9);
  Triangle T14(v6, v8, v10);
  Triangle T15(v8, v9, v10);
  Triangle T16(v6, v9, v10);

  p_app->AddTriangles({T13, T14, T15, T16});
}

void add_top_mountains(Application *p_app) {
  Vertex v1({-30, -50, 0}, {80, 80, 80});
  Vertex v2({-30, -30, 0}, {80, 80, 80});
  Vertex v3({-25, -45, 70}, {20, 40, 80});
  Vertex v4({-15, -35, 40}, {80, 20, 80});
  Vertex v5({-10, -50, 0}, {80, 80, 80});
  Vertex v6({0, -30, 0}, {80, 80, 80});
  Vertex v7({5, -45, 50}, {80, 80, 20});
  Vertex v8({20, -35, 120}, {20, 80, 80});
  Vertex v9({30, -50, 0}, {80, 80, 80});
  Vertex v10({30, -30, 0}, {80, 80, 80});

  Triangle T1(v1, v2, v3);
  Triangle T2(v1, v3, v5);
  Triangle T3(v2, v3, v5);
  Triangle T4(v1, v2, v5);

  p_app->AddTriangles({T1, T2, T3, T4});

  Triangle T5(v2, v4, v5);
  Triangle T6(v4, v5, v6);
  Triangle T7(v2, v4, v6);
  Triangle T8(v2, v5, v6);

  p_app->AddTriangles({T5, T6, T7, T8});

  Triangle T9(v5, v6, v7);
  Triangle T10(v5, v7, v9);
  Triangle T11(v6, v7, v9);
  Triangle T12(v5, v6, v9);

  p_app->AddTriangles({T9, T10, T11, T12});

  Triangle T13(v6, v8, v9);
  Triangle T14(v8, v9, v10);
  Triangle T15(v6, v8, v10);
  Triangle T16(v6, v9, v10);

  p_app->AddTriangles({T13, T14, T15, T16});
}

void add_bottom_mountains(Application *p_app) {
  Vertex v1({-30, 30, 0}, {80, 80, 80});
  Vertex v2({-30, 50, 0}, {80, 80, 80});
  Vertex v3({-25, 45, 55}, {255, 0, 0});
  Vertex v4({-15, 50, 0}, {80, 80, 80});
  Vertex v5({-15, 35, 50}, {255, 255, 0});
  Vertex v6({-10, 40, 60}, {0, 255, 0});
  Vertex v7({0, 35, 70}, {0, 255, 255});
  Vertex v8({15, 45, 110}, {255, 0, 0});
  Vertex v9({25, 40, 125}, {0, 0, 255});
  Vertex v10({30, 50, 0}, {80, 80, 80});
  Vertex v11({30, 30, 0}, {80, 80, 80});

  Triangle T1(v1, v2, v3);
  Triangle T2(v2, v3, v4);
  Triangle T3(v1, v3, v4);
  Triangle T4(v1, v2, v4);

  p_app->AddTriangles({T1, T2, T3, T4});

  Triangle T5(v1, v4, v5);
  Triangle T6(v4, v5, v6);
  Triangle T7(v4, v7, v11);
  Triangle T8(v4, v6, v7);
  Triangle T9(v1, v5, v7);
  Triangle T10(v1, v7, v11);
  Triangle T11(v5, v6, v7);
  Triangle T12(v1, v4, v11);

  p_app->AddTriangles({T5, T6, T7, T8, T9, T10, T11, T12});

  Triangle T13(v4, v8, v11);
  Triangle T14(v8, v9, v11);
  Triangle T15(v4, v8, v10);
  Triangle T16(v8, v9, v10);
  Triangle T17(v9, v10, v11);
  Triangle T18(v4, v10, v11);

  p_app->AddTriangles({T13, T14, T15, T16, T17, T18});
}

void add_corner(Application *p_app, const Vector3d &h) {
  Vertex v1({-10, -10, 0}, {80, 80, 80});
  Vertex v2({10, -10, 0}, {80, 80, 80});
  Vertex v3({10, 10, 0}, {80, 80, 80});
  Vertex v4({-10, 10, 0}, {80, 80, 80});
  Vertex v5({0, 0, 50}, {255, 255, 255});

  v1.point += h;
  v2.point += h;
  v3.point += h;
  v4.point += h;
  v5.point += h;

  Triangle T1(v1, v2, v5);
  Triangle T2(v2, v3, v5);
  Triangle T3(v3, v4, v5);
  Triangle T4(v1, v4, v5);
  Triangle T5(v1, v2, v4);
  Triangle T6(v2, v3, v4);

  p_app->AddTriangles({T1, T2, T3, T4, T5, T6});
}

void add_corners(Application *p_app) {
  add_corner(p_app, {-40, -40, 0});
  add_corner(p_app, {40, -40, 0});
  add_corner(p_app, {40, 40, 0});
  add_corner(p_app, {-40, 40, 0});
}

int main() {
  Application app;

  add_plane(&app);
  add_left_mountains(&app);
  add_right_mountains(&app);
  add_top_mountains(&app);
  add_bottom_mountains(&app);
  add_corners(&app);

  app.setCamera(0, 0, 10, 0, 1, 0);
  app.setPivot(0, 0, 1);
  app.setFrustum(-1, 1, -1, 1, 1, 1000);

  app.setScreen(640, 480);

  // app.setWireframeVisible(true);
  app.setFPSVisible(true);

  app.RunInteractiveScene();
  return 0;
}
