#pragma once

#include <ofMath.h>

#include <vector>

#include "Face.hpp"

namespace ofBoxel {
class PhysicsEngine {
 public:
  explicit PhysicsEngine(float boxelSize);
  std::vector<glm::ivec3> ray(glm::vec3 origin, glm::vec3 dir, float len) const;
  Face face(const glm::ivec3& target, const glm::ivec3& near) const;
  int boxelDistance(float posA, float posB) const;
  int boxelToLogic(float pos) const;
  glm::ivec3 boxelToLogic(const glm::vec3& pos) const;
  float logicToBoxel(int pos) const;
  glm::vec3 logicToBoxel(const glm::ivec3& pos) const;

 private:
  static int negativeSign(int a);
  float m_boxelSize;
  float m_boxelHalf;
};
}  // namespace ofBoxel