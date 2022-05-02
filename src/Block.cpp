#include "Block.hpp"

namespace ofBoxel {

Block::Block(const std::array<int, 6>& textureSlots) : m_textureSlots() {
  for (int i = 0; i < 6; i++) {
    m_textureSlots[i] = static_cast<float>(textureSlots[i]);
  }
}
float Block::getTextureSlotAt(int side) const {
  return m_textureSlots.at(side);
}
}  // namespace ofBoxel