#include "application.h"
#include "triangle.h"

#include "SFML/Graphics.hpp"

#include <string>

namespace application {

Application::Application() : is_visible_fps_(false) {}

Application::Application(const World &world, const Camera &camera,
                         const Screen &screen) {
  Create(world, camera, screen);
}

void Application::Create(const World &world, const Camera &camera,
                         const Screen &screen) {
  world_ = world;
  camera_ = camera;
  screen_ = screen;
  w_ = screen.getW();
  h_ = screen.getH();
  is_visible_fps_ = false;
}

void Application::AddTriangle(const Triangle &triangle) {
  world_.AddTriangle(triangle);
}

void Application::AddTriangles(const World::TriangleVec &triangles) {
  world_.AddTriangles(triangles);
}

void Application::setCamera(SpaceCoordsType x1, SpaceCoordsType y1,
                            SpaceCoordsType z1, SpaceCoordsType x2,
                            SpaceCoordsType y2, SpaceCoordsType z2) {
  camera_.setCamera(x1, y1, z1, x2, y2, z2);
}

void Application::setPivot(SpaceCoordsType x, SpaceCoordsType y,
                           SpaceCoordsType z) {
  camera_.setPivot(x, y, z);
}

void Application::setFrustum(BorderType l, BorderType r, BorderType b,
                             BorderType t, BorderType n, BorderType f) {
  camera_.setFrustum(l, r, b, t, n, f);
}

void Application::setScreen(Screen::SizeType w, Screen::SizeType h) {
  screen_.setScreen(w, h);
  w_ = w;
  h_ = h;
}

void Application::RunInteractiveScene(void) {
  Camera::AngleType angle_speed = 1.3;
  SpaceCoordsType move_speed = 20;

  InitWindow();
  clock_.restart();

  while (window_.isOpen()) {
    sf::Event event;

    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window_.close();
      }
    }

    float elapsed_time = clock_.getElapsedTime().asSeconds();
    Camera::AngleType true_angle_speed = angle_speed * elapsed_time;
    SpaceCoordsType true_move_speed = move_speed * elapsed_time;

    float framerate = getFramerate();
    clock_.restart();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
      camera_.Roll(-true_angle_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
      camera_.Roll(true_angle_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      camera_.Pitch(true_angle_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      camera_.Pitch(-true_angle_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      camera_.Yaw(true_angle_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      camera_.Yaw(-true_angle_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      camera_.MoveForward(true_move_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      camera_.MoveBackward(true_move_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      camera_.MoveRightward(true_move_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      camera_.MoveLeftward(true_move_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
      camera_.MoveUpward(true_move_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
      camera_.MoveDownward(true_move_speed);
    }

    renderer_.Render(world_, camera_, &screen_);
    Draw(framerate);
  }
}

void Application::RunRotatingScene(SpaceCoordsType radius,
                                   SpaceCoordsType speed) {
  SpaceCoordsType t = 0;
  InitWindow();
  clock_.restart();

  while (window_.isOpen()) {
    sf::Event event;

    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window_.close();
      }
    }

    SpaceCoordsType x = sin(t) * radius;
    SpaceCoordsType y = cos(t) * radius;

    setCamera(x, 0, y, -x, 0, -y);
    setPivot(0, 1, 0);
    setFrustum(-2, 2, -2, 2, 3, 100);

    float elapsed_time = clock_.getElapsedTime().asSeconds();
    float true_speed = speed * elapsed_time;
    t += true_speed;

    float framerate = getFramerate();
    clock_.restart();

    renderer_.Render(world_, camera_, &screen_);
    Draw(framerate);
  }
}

void Application::setFPSVisible(bool is_visible) {
  is_visible_fps_ = is_visible;
}

void Application::setWireframeVisible(bool is_visible) {
  renderer_.setWireframeVisible(is_visible);
}

void Application::setWireframeColor(const ColorType &color) {
  renderer_.setWireframeColor(color);
}

void Application::InitWindow() {
  window_.create(sf::VideoMode(w_, h_), "Renderer project",
                 sf::Style::Titlebar | sf::Style::Close);
}

void Application::Draw(float framerate) {
  window_.clear(ColorType::Black);

  sf::VertexArray pixels(sf::Points, w_ * h_);
  for (size_t i = 0; i < h_; ++i) {
    for (size_t j = 0; j < w_; ++j) {
      pixels[i * w_ + j] =
          sf::Vertex(sf::Vector2f(j, i), screen_.getColor(i, j));
    }
  }
  window_.draw(pixels);

  if (is_visible_fps_ && framerate) {
    sf::Font font;
    font.loadFromFile("../src/data/font.ttf");

    std::string sframerate = std::to_string(framerate);
    sframerate = sframerate.substr(0, sframerate.size() - 5);

    sf::Text text;
    text.setFont(font);
    text.setString(sframerate);
    text.setCharacterSize(18);
    text.setFillColor(ColorType::Blue);
    text.setStyle(sf::Text::Bold);

    window_.draw(text);
  }

  window_.display();
}

float Application::getFramerate() {
  return 1.f / clock_.getElapsedTime().asSeconds();
}

}  // namespace application
