#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  // シェーダー読み込み
  assert(m_shader.load("boxel.vert", "boxel.frag"));
  // 画像読み込み
  ofDisableArbTex();
  assert(m_image.load("debug.png"));
  // ワールドの生成
  ofMesh mesh = ofMesh::plane(1.0f, 1.0f, 2, 2, OF_PRIMITIVE_TRIANGLES);
  this->m_boxelRenderer =
      std::make_unique<ofBoxel::BoxelRenderer>(m_shader, mesh);
  for (int x = 0; x < 100; x += 2) {
    for (int y = 0; y < 100; y += 2) {
      for (int z = 0; z < 100; z += 2) {
        for (int i = 0; i < 6; i++) {
          m_boxelRenderer->batch(glm::vec3(x, y, z), i, i);
        }
      }
    }
  }
}

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
  m_shader.begin();
  m_shader.setUniformTexture("textureMap", m_image.getTexture(), 1);
  m_boxelRenderer->render();
  m_shader.end();
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
