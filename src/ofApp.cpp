#include "ofApp.h"

#include "World.hpp"

//--------------------------------------------------------------
void ofApp::setup() {
  // シェーダー読み込み
  assert(m_shader.load("boxel.vert", "boxel.frag"));
  // 画像読み込み
  ofDisableArbTex();
  assert(m_image.load("textureMap.png"));
  // ワールドの生成
  ofMesh mesh = ofMesh::plane(1.0f, 1.0f, 2, 2, OF_PRIMITIVE_TRIANGLES);
  this->m_boxelRenderer =
      std::make_unique<ofBoxel::BoxelRenderer>(m_shader, mesh);
  auto dirt =
      std::make_shared<ofBoxel::Block>(std::array<int, 6>{0, 0, 0, 0, 0, 0});
  auto grass =
      std::make_shared<ofBoxel::Block>(std::array<int, 6>{2, 0, 1, 1, 1, 1});
  ofBoxel::World world(glm::ivec3(64, 64, 64));
  world.setBlock(glm::ivec3(0, 0, 0), grass);
  world.setBlock(glm::ivec3(1, 0, 0), grass);
  world.setBlock(glm::ivec3(2, 0, 0), grass);
  world.batch(*(m_boxelRenderer.get()));
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
