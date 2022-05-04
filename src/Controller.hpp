#pragma once
#include <ofCamera.h>

namespace ofBoxel {
class Controller {
 public:
  explicit Controller(ofNode& node);
  ~Controller();

  glm::vec3 getForward() const;
  glm::vec3 getBackward() const;
  glm::vec3 getLeft() const;
  glm::vec3 getRight() const;

  bool enabledMove;
  bool enabledRotation;
  float rotationSpeed;
  float moveSpeed;
  float angleLimit;
  int forwardKey;
  int backwardKey;
  int leftKey;
  int rightKey;
  int upKey;
  int downKey;

 protected:
  ofNode& m_node;
  std::array<bool, 6> m_keyState;
  glm::vec2 m_mousePos;
  glm::vec2 m_mouseDiff;
  glm::vec3 m_eularAngles;
  bool m_rotationStart;
  void update(ofEventArgs& e);
  void keyPressed(ofKeyEventArgs& e);
  void keyReleased(ofKeyEventArgs& e);
  void mouseMoved(ofMouseEventArgs& e);
  void mouseDragged(ofMouseEventArgs& e);
};
}  // namespace ofBoxel