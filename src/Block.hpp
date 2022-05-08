#pragma once
#include <array>
#include <memory>

#include "Shape.hpp"

namespace ofBoxel {
class Block {
 public:
  explicit Block(Shape shape, const std::array<int, 6>& textureSlots);
  float getTextureSlotAt(int side) const;
  Shape getShape() const;

 private:
  Shape m_shape;
  std::array<float, 6> m_textureSlots;
};
using BlockInstance = std::shared_ptr<Block>;
}  // namespace ofBoxel