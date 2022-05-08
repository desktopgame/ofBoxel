#include "Block.hpp"

namespace ofBoxel {

Block::Block(Shape shape, const std::array<int, 6>& textureSlots)
    : m_shape(shape), m_textureSlots() {
  for (int i = 0; i < 6; i++) {
    m_textureSlots[i] = static_cast<float>(textureSlots[i]);
  }
}
float Block::getTextureSlotAt(int side) const {
  return m_textureSlots.at(side);
}
Shape Block::getShape() const { return m_shape; }
}  // namespace ofBoxel