#include "ofApp.h"

#include "PerlinNoise.hpp"
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
  uint64_t start = ofGetElapsedTimeMillis();
  const int worldSize = 128;
  const int freq = 4;
  const int octave = 2;
  const double fx = static_cast<double>(freq) / static_cast<double>(worldSize);
  const double fz = static_cast<double>(freq) / static_cast<double>(worldSize);
  this->m_world = std::make_unique<ofBoxel::World>(
      glm::ivec3(worldSize, worldSize, worldSize));
  siv::PerlinNoise perilinNoise;
  perilinNoise.reseed(static_cast<unsigned int>(ofRandom(1000)));
  for (int x = 0; x < worldSize; x++) {
    for (int z = 0; z < worldSize; z++) {
      double nx = static_cast<double>(x) * fx;
      double nz = static_cast<double>(z) * fz;
      double y = perilinNoise.octaveNoise0_1(nx, nz, octave);
      int iy = static_cast<int>(static_cast<double>(worldSize) * y);
      if (iy < 0)
        iy = 0;
      else if (iy >= worldSize)
        iy = worldSize - 1;
      m_world->setBlock(glm::ivec3(x, iy, z), grass);
      for (int down = iy - 1; down >= 0; down--) {
        m_world->setBlock(glm::ivec3(x, down, z), dirt);
      }
    }
  }
  m_world->batch(*(m_boxelRenderer.get()));
  uint64_t end = ofGetElapsedTimeMillis();
  ofLog() << (end - start) << "ms";
  // カメラ指定
  m_camera.setPosition(glm::vec3(64, 80, 64));
  this->m_controller = std::make_unique<ofBoxel::Controller>(m_camera);
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
