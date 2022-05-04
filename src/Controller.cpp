#include "Controller.hpp"

#include <ofAppRunner.h>
#include <ofEventUtils.h>
#include <ofGraphics.h>
#include <ofLog.h>
#include <ofMath.h>
#include <ofMatrixStack.h>
#include <ofUtils.h>

#include <glm/gtc/quaternion.hpp>
namespace ofBoxel {

Controller::Controller(ofNode& node)
    : enabledMove(true),
      enabledRotation(true),
      rotationSpeed(5.0f),
      moveSpeed(90.0f),
      angleLimit(1.5f),
      forwardKey('w'),
      backwardKey('s'),
      leftKey('a'),
      rightKey('d'),
      upKey(OF_KEY_UP),
      downKey(OF_KEY_DOWN),
      m_node(node),
      m_keyState(),
      m_mousePos(),
      m_mouseDiff(),
      m_eularAngles(),
      m_rotationStart() {
  auto& events = ofEvents();
  ofAddListener(events.update, this, &Controller::update,
                OF_EVENT_ORDER_BEFORE_APP);
  ofAddListener(events.keyPressed, this, &Controller::keyPressed,
                OF_EVENT_ORDER_BEFORE_APP);
  ofAddListener(events.keyReleased, this, &Controller::keyReleased,
                OF_EVENT_ORDER_BEFORE_APP);
  ofAddListener(events.mouseMoved, this, &Controller::mouseMoved,
                OF_EVENT_ORDER_BEFORE_APP);
  ofAddListener(events.mouseDragged, this, &Controller::mouseDragged,
                OF_EVENT_ORDER_BEFORE_APP);
}

Controller::~Controller() {
  auto& events = ofEvents();
  ofRemoveListener(events.update, this, &Controller::update,
                   OF_EVENT_ORDER_BEFORE_APP);
  ofRemoveListener(events.keyPressed, this, &Controller::keyPressed,
                   OF_EVENT_ORDER_BEFORE_APP);
  ofRemoveListener(events.keyReleased, this, &Controller::keyReleased,
                   OF_EVENT_ORDER_BEFORE_APP);
  ofRemoveListener(events.mouseMoved, this, &Controller::mouseMoved,
                   OF_EVENT_ORDER_BEFORE_APP);
  ofRemoveListener(events.mouseDragged, this, &Controller::mouseDragged,
                   OF_EVENT_ORDER_BEFORE_APP);
}

glm::vec3 Controller::getForward() const {
  auto look = m_node.getLookAtDir();
  look.y = 0;
  return glm::normalize(look);
}
glm::vec3 Controller::getBackward() const { return -getForward(); }
glm::vec3 Controller::getLeft() const { return -getRight(); }
glm::vec3 Controller::getRight() const {
  auto side = m_node.getSideDir();
  side.y = 0;
  return glm::normalize(side);
}
// protected

void Controller::update(ofEventArgs& e) {
  float delta = static_cast<float>(ofGetLastFrameTime());
  // 上下移動
  glm::vec3 offset;
  if (m_keyState[0]) {
    offset += glm::vec3(0, 1, 0) * moveSpeed * delta;
  } else if (m_keyState[1]) {
    offset -= glm::vec3(0, 1, 0) * moveSpeed * delta;
  }
  // 前後左右
  if (m_keyState[2]) {
    offset += getForward() * moveSpeed * delta;
  } else if (m_keyState[3]) {
    offset += getBackward() * moveSpeed * delta;
  } else if (m_keyState[4]) {
    offset += getLeft() * moveSpeed * delta;
  } else if (m_keyState[5]) {
    offset += getRight() * moveSpeed * delta;
  }
  m_node.move(offset);
  // 回転
  // マウスの横(X)が画面の縦(Y)
  glm::quat rotation = m_node.getOrientationQuat();
  glm::quat angle = glm::identity<glm::quat>();
  glm::vec2 mouseDiffA = glm::abs(m_mouseDiff);
  if (mouseDiffA.x > mouseDiffA.y) {
    m_eularAngles.y += m_mouseDiff.x * (rotationSpeed * delta) * -1;
  } else if (mouseDiffA.y > mouseDiffA.x) {
    m_eularAngles.x += m_mouseDiff.y * (rotationSpeed * delta) * -1;
    m_eularAngles.x = ofClamp(m_eularAngles.x, -angleLimit, angleLimit);
  }
  glm::quat angleX = glm::angleAxis(m_eularAngles.x, glm::vec3(1, 0, 0));
  glm::quat angleY = glm::angleAxis(m_eularAngles.y, glm::vec3(0, 1, 0));
  m_node.setOrientation(angleY * angleX);
  this->m_mouseDiff = glm::vec2(0, 0);
}
void Controller::keyPressed(ofKeyEventArgs& e) {
  if (!enabledMove) {
    return;
  }
  if (e.key == upKey) {
    m_keyState[0] = true;
  } else if (e.key == downKey) {
    m_keyState[1] = true;
  } else if (e.key == forwardKey) {
    m_keyState[2] = true;
  } else if (e.key == backwardKey) {
    m_keyState[3] = true;
  } else if (e.key == leftKey) {
    m_keyState[4] = true;
  } else if (e.key == rightKey) {
    m_keyState[5] = true;
  }
}
void Controller::keyReleased(ofKeyEventArgs& e) {
  if (!enabledMove) {
    return;
  }
  if (e.key == upKey) {
    m_keyState[0] = false;
  } else if (e.key == downKey) {
    m_keyState[1] = false;
  } else if (e.key == forwardKey) {
    m_keyState[2] = false;
  } else if (e.key == backwardKey) {
    m_keyState[3] = false;
  } else if (e.key == leftKey) {
    m_keyState[4] = false;
  } else if (e.key == rightKey) {
    m_keyState[5] = false;
  }
}
void Controller::mouseMoved(ofMouseEventArgs& e) {
  if (!enabledRotation) {
    return;
  }
  if (m_rotationStart) {
    this->m_mouseDiff = e - m_mousePos;
    this->m_mousePos.x = e.x;
    this->m_mousePos.y = e.y;
  } else {
    this->m_mousePos.x = e.x;
    this->m_mousePos.y = e.y;
    this->m_rotationStart = true;
  }
}
void Controller::mouseDragged(ofMouseEventArgs& e) {}
}  // namespace ofBoxel
