#include "screen.h"

#include "SFML/Graphics/Color.hpp"

#include <cassert>
#include <vector>

namespace application {

const SpaceCoordsType Screen::kMaxZValue_ = 1;

Screen::Screen(SizeType w, SizeType h) { setScreen(w, h); }

void Screen::setScreen(SizeType w, SizeType h) {
  pixel_count_ = w * h;
  w_ = w;
  h_ = h;
  clear();
}

void Screen::setPixel(ScreenCoordsType i, ScreenCoordsType j, SpaceCoordsType z,
                      sf::Uint8 red, sf::Uint8 green, sf::Uint8 blue,
                      sf::Uint8 alpha) {
  assert(isCorrectPixel(i, j) &&
         "(i, j) does not belong to [0, h - 1]x[0, w - 1]");

  const ScreenCoordsType index = getIndex(i, j);
  z_buffer_[index] = z;
  c_buffer_[index] = ColorType(red, green, blue, alpha);
}

SpaceCoordsType Screen::getZ(ScreenCoordsType i, ScreenCoordsType j) const {
  assert(isCorrectPixel(i, j) &&
         "(i, j) does not belong to [0, h - 1]x[0, w - 1]");
  return z_buffer_[getIndex(i, j)];
}

ColorType Screen::getColor(ScreenCoordsType i, ScreenCoordsType j) const {
  assert(isCorrectPixel(i, j) &&
         "(i, j) does not belong to [0, h - 1]x[0, w - 1]");
  return c_buffer_[getIndex(i, j)];
}

Screen::SizeType Screen::getW() const { return w_; }

Screen::SizeType Screen::getH() const { return h_; }

void Screen::clear() {
  z_buffer_.assign(pixel_count_, kMaxZValue_);
  c_buffer_.assign(pixel_count_, ColorType::Black);
  blocked_.assign(pixel_count_, false);
}

Screen::ScreenCoordsType Screen::getIndex(ScreenCoordsType i,
                                          ScreenCoordsType j) const {
  return i * w_ + j;
}

bool Screen::isCorrectPixel(ScreenCoordsType i, ScreenCoordsType j) const {
  return i >= 0 && j >= 0 && i < h_ && j < w_;
}

void Screen::BlockPixel(ScreenCoordsType i, ScreenCoordsType j) {
  blocked_[getIndex(i, j)] = true;
}

bool Screen::isBlockedPixel(ScreenCoordsType i, ScreenCoordsType j) const {
  return blocked_[getIndex(i, j)];
}

}  // namespace application
