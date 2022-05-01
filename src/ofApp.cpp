#include "ofApp.h"

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
  // 地面
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 6; j++) {
      m_boxelRenderer->batch(glm::vec3(0, 0, -i), 2, 2, 1);
    }
    for (int j = 0; j < 6; j++) {
      m_boxelRenderer->batch(glm::vec3(i, 0, -9), 5, 5, 1);
    }
    for (int j = 0; j < 6; j++) {
      m_boxelRenderer->batch(glm::vec3(i, 0, 0), 4, 4, 1);
    }
    for (int j = 0; j < 6; j++) {
      m_boxelRenderer->batch(glm::vec3(9, 0, -i), 3, 3, 1);
    }
    for (int j = 0; j < 10; j++) {
      for (int k = 0; k < 6; k++) {
        m_boxelRenderer->batch(glm::vec3(j, 0, -i), 0, 0, 2);
      }
    }
  }
  // 木
  for (int i = 0; i < 4; i++) {
    // 上と下はいらないですが、サンプルなので手抜き
    for (int k = 0; k < 6; k++) {
      m_boxelRenderer->batch(glm::vec3(4, 1 + i, -4), k, k, 8);
    }
  }
  // 葉っぱ
  for (int x = 3; x < 6; x++) {
    for (int z = -3; z > -6; z--) {
      for (int y = 0; y < 3; y++) {
        for (int t = 0; t < 6; t++) {
          m_boxelRenderer->batch(glm::vec3(x, 5 + y, z), t, t, 16);
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
