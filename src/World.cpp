#include "World.hpp"

#include "BoxelRenderer.hpp"

namespace ofBoxel {

World::World(const glm::ivec3& size) : m_size(size), m_dirtyPositions() {
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
  if (m_dirtyPositions.empty()) {
    boxelRenderer.clear();
    // 全部変更されていたらゼロからバッチ
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
  } else {
    // 先に重複を削除
    auto iter = std::unique(m_dirtyPositions.begin(), m_dirtyPositions.end());
    m_dirtyPositions.erase(iter, m_dirtyPositions.end());
    // 少しだけ変更されているなら必要に応じてパッチ
    boxelRenderer.compact(m_dirtyPositions);
    for (auto dp : m_dirtyPositions) {
      batch(boxelRenderer, dp);
    }
  }
  m_dirtyPositions.clear();
}
void World::batch(BoxelRenderer& boxelRenderer, const glm::ivec3& pos) {
  auto block = getBlock(pos);
  if (!block) {
    return;
  }
  switch (block->getShape()) {
    case Shape::Block: {
      if (!isFilled(pos - glm::ivec3(1, 0, 0))) {
        boxelRenderer.batch(pos, 2, 0, 2, block->getTextureSlotAt(2));
      }
      if (!isFilled(pos + glm::ivec3(1, 0, 0))) {
        boxelRenderer.batch(pos, 3, 0, 3, block->getTextureSlotAt(3));
      }
      if (!isFilled(pos - glm::ivec3(0, 0, 1))) {
        boxelRenderer.batch(pos, 5, 0, 5, block->getTextureSlotAt(5));
      }
      if (!isFilled(pos + glm::ivec3(0, 0, 1))) {
        boxelRenderer.batch(pos, 4, 0, 4, block->getTextureSlotAt(4));
      }
      if (!isFilled(pos + glm::ivec3(0, 1, 0))) {
        boxelRenderer.batch(pos, 0, 0, 0, block->getTextureSlotAt(0));
      }
      if (!isFilled(pos - glm::ivec3(0, 1, 0))) {
        boxelRenderer.batch(pos, 1, 0, 1, block->getTextureSlotAt(1));
      }
      break;
    }
    case Shape::TopSlab: {
      int offset = 6;
      int scale = 1;
      if (!isFilled(pos - glm::ivec3(1, 0, 0))) {
        boxelRenderer.batch(pos, 2 + offset, scale, 2,
                            block->getTextureSlotAt(2));
      }
      if (!isFilled(pos + glm::ivec3(1, 0, 0))) {
        boxelRenderer.batch(pos, 3 + offset, scale, 3,
                            block->getTextureSlotAt(3));
      }
      if (!isFilled(pos - glm::ivec3(0, 0, 1))) {
        boxelRenderer.batch(pos, 5 + offset, scale, 5,
                            block->getTextureSlotAt(5));
      }
      if (!isFilled(pos + glm::ivec3(0, 0, 1))) {
        boxelRenderer.batch(pos, 4 + offset, scale, 4,
                            block->getTextureSlotAt(4));
      }
      if (!isFilled(pos + glm::ivec3(0, 1, 0))) {
        boxelRenderer.batch(pos, 0 + offset, 0, 0, block->getTextureSlotAt(0));
      }
      if (!isFilled(pos - glm::ivec3(0, 1, 0))) {
        boxelRenderer.batch(pos, 1 + offset, 0, 1, block->getTextureSlotAt(1));
      }
      break;
    }
    case Shape::BottomSlab: {
      int offset = 12;
      int scale = 1;
      if (!isFilled(pos - glm::ivec3(1, 0, 0))) {
        boxelRenderer.batch(pos, 2 + offset, scale, 2,
                            block->getTextureSlotAt(2));
      }
      if (!isFilled(pos + glm::ivec3(1, 0, 0))) {
        boxelRenderer.batch(pos, 3 + offset, scale, 3,
                            block->getTextureSlotAt(3));
      }
      if (!isFilled(pos - glm::ivec3(0, 0, 1))) {
        boxelRenderer.batch(pos, 5 + offset, scale, 5,
                            block->getTextureSlotAt(5));
      }
      if (!isFilled(pos + glm::ivec3(0, 0, 1))) {
        boxelRenderer.batch(pos, 4 + offset, scale, 4,
                            block->getTextureSlotAt(4));
      }
      if (!isFilled(pos + glm::ivec3(0, 1, 0))) {
        boxelRenderer.batch(pos, 0 + offset, 0, 0, block->getTextureSlotAt(0));
      }
      if (!isFilled(pos - glm::ivec3(0, 1, 0))) {
        boxelRenderer.batch(pos, 1 + offset, 0, 1, block->getTextureSlotAt(1));
      }
      break;
    }
    case Shape::LeftSlab: {
      int offset = 18;
      int scale = 2;
      if (!isFilled(pos - glm::ivec3(1, 0, 0))) {
        boxelRenderer.batch(pos, 2 + offset, 0, 2, block->getTextureSlotAt(2));
      }
      if (!isFilled(pos + glm::ivec3(1, 0, 0))) {
        boxelRenderer.batch(pos, 3 + offset, 0, 3, block->getTextureSlotAt(3));
      }
      if (!isFilled(pos - glm::ivec3(0, 0, 1))) {
        boxelRenderer.batch(pos, 5 + offset, scale, 5,
                            block->getTextureSlotAt(5));
      }
      if (!isFilled(pos + glm::ivec3(0, 0, 1))) {
        boxelRenderer.batch(pos, 4 + offset, scale, 4,
                            block->getTextureSlotAt(4));
      }
      if (!isFilled(pos + glm::ivec3(0, 1, 0))) {
        boxelRenderer.batch(pos, 0 + offset, scale, 0,
                            block->getTextureSlotAt(0));
      }
      if (!isFilled(pos - glm::ivec3(0, 1, 0))) {
        boxelRenderer.batch(pos, 1 + offset, scale, 1,
                            block->getTextureSlotAt(1));
      }
      break;
    }
    case Shape::RightSlab: {
      int offset = 24;
      int scale = 2;
      if (!isFilled(pos - glm::ivec3(1, 0, 0))) {
        boxelRenderer.batch(pos, 2 + offset, 0, 2, block->getTextureSlotAt(2));
      }
      if (!isFilled(pos + glm::ivec3(1, 0, 0))) {
        boxelRenderer.batch(pos, 3 + offset, 0, 3, block->getTextureSlotAt(3));
      }
      if (!isFilled(pos - glm::ivec3(0, 0, 1))) {
        boxelRenderer.batch(pos, 5 + offset, scale, 5,
                            block->getTextureSlotAt(5));
      }
      if (!isFilled(pos + glm::ivec3(0, 0, 1))) {
        boxelRenderer.batch(pos, 4 + offset, scale, 4,
                            block->getTextureSlotAt(4));
      }
      if (!isFilled(pos + glm::ivec3(0, 1, 0))) {
        boxelRenderer.batch(pos, 0 + offset, scale, 0,
                            block->getTextureSlotAt(0));
      }
      if (!isFilled(pos - glm::ivec3(0, 1, 0))) {
        boxelRenderer.batch(pos, 1 + offset, scale, 1,
                            block->getTextureSlotAt(1));
      }
      break;
    }
  }
}
void World::invalidate(const glm::ivec3& pos) {
  glm::ivec3 a[7];
  a[0] = (pos);
  a[1] = (pos + glm::ivec3(0, 1, 0));   // top
  a[2] = (pos + glm::ivec3(0, -1, 0));  // bottom
  a[3] = (pos + glm::ivec3(-1, 0, 0));  // left
  a[4] = (pos + glm::ivec3(1, 0, 0));   // right
  a[5] = (pos + glm::ivec3(0, 0, 1));   // front
  a[6] = (pos + glm::ivec3(0, 0, -1));  // back
  for (auto e : a) {
    if (isContains(e)) {
      m_dirtyPositions.emplace_back(e);
    }
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
  auto block = getBlock(pos);
  return block != nullptr && block->getShape() == Shape::Block;
}
}  // namespace ofBoxel