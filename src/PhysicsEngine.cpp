#include "PhysicsEngine.hpp"

#include <ofLog.h>

#include <array>
#include <cmath>
#include <glm/glm.hpp>
#include <limits>

namespace ofBoxel {

PhysicsEngine::PhysicsEngine(float boxelSize)
    : m_boxelSize(boxelSize), m_boxelHalf(boxelSize / 2.0f) {}

std::vector<glm::ivec3> PhysicsEngine::ray(glm::vec3 origin, glm::vec3 dir,
                                           float len) const {
  // 参考: Voxel Traversal Algorithm (Ray Casting)
  // https://www.gamedev.net/blogs/entry/2265248-voxel-traversal-algorithm-ray-casting/
  std::vector<glm::ivec3> points;
  glm::vec3 offseted = origin + (glm::vec3(1, 1, 1) * (m_boxelSize / 2.0f));
  glm::ivec3 index = boxelToLogic(offseted);
  glm::vec3 end = boxelToLogic(offseted + (dir * len));
  glm::ivec3 step = glm::sign(dir);
  glm::vec3 next =
      glm::vec3{(index.x + negativeSign(step.x) + 1) * m_boxelSize,
                (index.y + negativeSign(step.y) + 1) * m_boxelSize,
                (index.z + negativeSign(step.z) + 1) * m_boxelSize};
  glm::vec3 max = (next - offseted) / dir;
  glm::vec3 delta = glm::vec3(step) * m_boxelSize / dir;
  int distance = std::abs(end.x - index.x) + std::abs(end.y - index.y) +
                 std::abs(end.z - index.z);
  while (distance > 0) {
    if (max.x < max.y && max.x < max.z) {
      index.x += step.x;
      max.x += delta.x;
    } else if (max.y < max.z) {
      index.y += step.y;
      max.y += delta.y;
    } else {
      index.z += step.z;
      max.z += delta.z;
    }
    points.emplace_back(index);
    distance--;
  }
  // 重複の削除
  auto iter =
      std::unique(points.begin(), points.end(), [](glm::ivec3 a, glm::ivec3 b) {
        return a.x == b.x && a.y == b.y && a.z == b.z;
      });
  points.erase(iter, points.end());
  return points;
}

Face PhysicsEngine::face(const glm::ivec3& target,
                         const glm::ivec3& near) const {
  if (target.x != near.x) {
    if (near.x < target.x) {
      return Face::Left;
    } else {
      return Face::Right;
    }
  } else if (target.y != near.y) {
    if (near.y > target.y) {
      return Face::Top;
    } else {
      return Face::Bottom;
    }
  } else if (target.z != near.z) {
    if (near.z > target.z) {
      return Face::Front;
    } else {
      return Face::Back;
    }
  } else {
    return static_cast<Face>(-1);
  }
}

int PhysicsEngine::boxelDistance(float posA, float posB) const {
  float bigger = std::max(posA, posB);
  float smaller = std::min(posA, posB);
  return boxelToLogic(bigger) - boxelToLogic(smaller);
}

int PhysicsEngine::boxelToLogic(float pos) const {
  if (pos < 0) {
    return -boxelToLogic(-pos);
  }
  // この描画システムはサイズ1のボクセルを-0.5~0.5の範囲に描画するようになっており、
  // 整数は必ずボクセルの中心になる
  // なので半ボクセル分ずらしてから切り捨てで論理座標を取得できる
  float offset = ((m_boxelSize / 2.0f) * glm::sign(pos));
  return static_cast<int>(::floor(pos + offset));
}

glm::ivec3 PhysicsEngine::boxelToLogic(const glm::vec3& pos) const {
  return glm::ivec3(boxelToLogic(pos.x), boxelToLogic(pos.y),
                    boxelToLogic(pos.z));
}

float PhysicsEngine::logicToBoxel(int pos) const {
  return (static_cast<float>(pos) * m_boxelSize);
}

glm::vec3 PhysicsEngine::logicToBoxel(const glm::ivec3& pos) const {
  return glm::vec3(logicToBoxel(pos.x), logicToBoxel(pos.y),
                   logicToBoxel(pos.z));
}

int PhysicsEngine::negativeSign(int a) {
  if (a >= 0) {
    return 0;
  }
  return -1;
}
}  // namespace ofBoxel