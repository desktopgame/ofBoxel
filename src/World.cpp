#include "World.hpp"

#include "BoxelRenderer.hpp"

namespace ofBoxel {

World::World(const glm::ivec3& size) : m_size(size) {
  for (int i = 0; i < size.y; i++) {
    std::vector<std::vector<BlockInstance>> v1;
    for (int j = 0; j < size.x; j++) {
      std::vector<BlockInstance> v2;
      for (int k = 0; k < size.z; k++) {
        v2.emplace_back(nullptr);
      }
      v1.emplace_back(v2);
    }
    m_table.emplace_back(v1);
  }
}
void World::batch(BoxelRenderer& boxelRenderer) {
  glm::ivec3 p;
  for (int i = 0; i < m_size.y; i++) {
    p.y = i;
    for (int j = 0; j < m_size.x; j++) {
      p.x = j;
      for (int k = 0; k < m_size.z; k++) {
        p.z = k;
        batch(boxelRenderer, p);
      }
    }
  }
}
void World::batch(BoxelRenderer& boxelRenderer, const glm::ivec3& pos) {
  auto block = getBlock(pos);
  if (!block) {
    return;
  }
  if (!isFilled(pos - glm::ivec3(1, 0, 0))) {
    boxelRenderer.batch(pos, 2, 2, block->getTextureSlotAt(2));
  }
  if (!isFilled(pos + glm::ivec3(1, 0, 0))) {
    boxelRenderer.batch(pos, 3, 3, block->getTextureSlotAt(3));
  }
  if (!isFilled(pos - glm::ivec3(0, 0, 1))) {
    boxelRenderer.batch(pos, 5, 5, block->getTextureSlotAt(5));
  }
  if (!isFilled(pos + glm::ivec3(0, 0, 1))) {
    boxelRenderer.batch(pos, 4, 4, block->getTextureSlotAt(4));
  }
  if (!isFilled(pos + glm::ivec3(0, 1, 0))) {
    boxelRenderer.batch(pos, 0, 0, block->getTextureSlotAt(0));
  }
  if (!isFilled(pos - glm::ivec3(0, 1, 0))) {
    boxelRenderer.batch(pos, 1, 1, block->getTextureSlotAt(1));
  }
}
void World::setBlock(const glm::ivec3& pos, const BlockInstance& block) {
  m_table[pos.y][pos.x][pos.z] = block;
}
BlockInstance World::getBlock(const glm::ivec3& pos) const {
  return m_table[pos.y][pos.x][pos.z];
}
bool World::isContains(const glm::ivec3& pos) const {
  if (pos.y >= m_size.y || pos.y < 0) {
    return false;
  }
  if (pos.x >= m_size.x || pos.x < 0) {
    return false;
  }
  if (pos.z >= m_size.z || pos.z < 0) {
    return false;
  }
  return true;
}
bool World::isFilled(const glm::ivec3& pos) const {
  if (!isContains(pos)) {
    return false;
  }
  return getBlock(pos) != nullptr;
}
}  // namespace ofBoxel