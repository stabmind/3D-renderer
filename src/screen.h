#pragma once

#include "renderer_types.h"

#include "SFML/Graphics/Color.hpp"

#include <vector>

namespace application {

class Screen {
public:
  using SpaceCoordsType = renderer_types::SpaceCoordsType;
  using ScreenCoordsType = int;
  using SizeType = size_t;
  using ColorType = renderer_types::ColorType;

  Screen() = default;
  Screen(SizeType w, SizeType h);

  void setScreen(SizeType w, SizeType h);

  void setPixel(ScreenCoordsType i, ScreenCoordsType j, SpaceCoordsType z,
                sf::Uint8 red, sf::Uint8 green, sf::Uint8 blue,
                sf::Uint8 alpha = 255);

  void BlockPixel(ScreenCoordsType i, ScreenCoordsType j);
  bool isBlockedPixel(ScreenCoordsType i, ScreenCoordsType j) const;

  SpaceCoordsType getZ(ScreenCoordsType i, ScreenCoordsType j) const;
  ColorType getColor(ScreenCoordsType i, ScreenCoordsType j) const;
  SizeType getW(void) const;
  SizeType getH(void) const;

  void clear(void);

private:
  ScreenCoordsType getIndex(ScreenCoordsType i, ScreenCoordsType j) const;
  bool isCorrectPixel(ScreenCoordsType i, ScreenCoordsType j) const;

  static const SpaceCoordsType kMaxZValue_;

  SizeType pixel_count_;
  SizeType w_, h_;
  std::vector<SpaceCoordsType> z_buffer_;
  std::vector<ColorType> c_buffer_;

  std::vector<int> blocked_;
};

} // namespace application
