#pragma once
#include <array>
#include <memory>

namespace ofBoxel {
class Block {
 public:
  explicit Block(const std::array<int, 6>& textureSlots);
  float getTextureSlotAt(int side) const;

 private:
  std::array<float, 6> m_textureSlots;
};
using BlockInstance = std::shared_ptr<Block>;
}  // namespace ofBoxel