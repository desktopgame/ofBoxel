#pragma once

#include <memory>

#include "BoxelRenderer.hpp"
#include "Controller.hpp"
#include "World.hpp"
#include "ofMain.h"

class ofApp : public ofBaseApp {
 public:
  void setup();
  void update();
  void draw();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);

 private:
  void lockMouseCursor();
  void unlockMouseCursor();
  ofCamera m_camera;
  ofShader m_boxelShader;
  ofShader m_rayShader;
  ofImage m_image;
  bool m_openInv;
  std::vector<glm::ivec3> m_ray;
  std::unique_ptr<ofBoxel::Controller> m_controller;
  std::unique_ptr<ofBoxel::World> m_world;
  std::unique_ptr<ofBoxel::BoxelRenderer> m_boxelRenderer;
  std::unique_ptr<ofBoxel::BoxelRenderer> m_rayRenderer;
};
