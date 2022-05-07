#pragma once
#include <ofVectorMath.h>

#include <vector>

#include "Block.hpp"

namespace ofBoxel {
class BoxelRenderer;
class World {
 public:
  explicit World(const glm::ivec3& size);
  void batch(BoxelRenderer& boxelRenderer);
  void batch(BoxelRenderer& boxelRenderer, const glm::ivec3& pos);
  void invalidate(const glm::ivec3& pos);
  void setBlock(const glm::ivec3& pos, const BlockInstance& block);
  BlockInstance getBlock(const glm::ivec3& pos) const;
  bool isContains(const glm::ivec3& pos) const;
  bool isFilled(const glm::ivec3& pos) const;

 private:
  glm::ivec3 m_size;
  std::vector<std::vector<std::vector<BlockInstance>>> m_table;
  std::vector<glm::ivec3> m_dirtyPositions;
};
}  // namespace ofBoxel