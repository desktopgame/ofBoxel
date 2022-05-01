#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
  glDisable(GL_CULL_FACE);
  // FPSの描画
  {
    char buf[128];
    ::sprintf(buf, "%.2f", ofGetFrameRate());
    ofDrawBitmapString(buf, glm::vec2(50, 50));
  }
  m_camera.begin();
  // ワールドの描画
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  ofEnableDepthTest();
  for (int x = 0; x < 100; x += 2) {
    for (int y = 0; y < 100; y += 2) {
      for (int z = 0; z < 100; z += 2) {
        ofBox(glm::vec3(x, y, z), 1.0f);
      }
    }
  }
  m_camera.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}
