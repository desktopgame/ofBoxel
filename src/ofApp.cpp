#include "ofApp.h"

#include <GLFW/glfw3.h>
#include <ofAppGLFWWindow.h>

#include "PerlinNoise.hpp"
#include "PhysicsEngine.hpp"
#include "World.hpp"
//--------------------------------------------------------------
void ofApp::setup() {
  // シェーダー読み込み
  assert(m_boxelShader.load("boxel.vert", "boxel.frag"));
  assert(m_rayShader.load("boxel.vert", "boxel.frag"));
  // 画像読み込み
  ofDisableArbTex();
  assert(m_image.load("textureMap.png"));
  // ワールドの生成
  ofMesh boxMesh = ofMesh::plane(1.0f, 1.0f, 2, 2, OF_PRIMITIVE_TRIANGLES);
  ofMesh rayMesh = ofMesh::plane(1.2f, 1.2f, 2, 2, OF_PRIMITIVE_TRIANGLES);
  this->m_boxelRenderer =
      std::make_unique<ofBoxel::BoxelRenderer>(m_boxelShader, boxMesh);
  this->m_rayRenderer =
      std::make_unique<ofBoxel::BoxelRenderer>(m_rayShader, rayMesh, 0.6f);
  this->m_dirt = std::make_shared<ofBoxel::Block>(
      ofBoxel::Shape::Block, std::array<int, 6>{0, 0, 0, 0, 0, 0});
  this->m_grass = std::make_shared<ofBoxel::Block>(
      ofBoxel::Shape::Block, std::array<int, 6>{2, 0, 1, 1, 1, 1});
  this->m_stone = std::make_shared<ofBoxel::Block>(
      ofBoxel::Shape::FrontSlab, std::array<int, 6>{24, 24, 24, 24, 24, 24});
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
      m_world->setBlock(glm::ivec3(x, iy, z), m_grass);
      for (int down = iy - 1; down >= 0; down--) {
        m_world->setBlock(glm::ivec3(x, down, z), m_dirt);
      }
    }
  }
  m_world->batch(*(m_boxelRenderer.get()));
  uint64_t end = ofGetElapsedTimeMillis();
  ofLog() << (end - start) << "ms";
  // カメラ指定
  m_camera.setPosition(glm::vec3(64, 80, 64));
  this->m_controller = std::make_unique<ofBoxel::Controller>(m_camera);
  this->m_openInv = false;
  lockMouseCursor();
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
  //
  // 3D描画
  //
  m_camera.begin();
  // ワールドの描画
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  ofEnableDepthTest();
  m_boxelShader.begin();
  m_boxelShader.setUniformTexture("textureMap", m_image.getTexture(), 1);
  m_boxelRenderer->render();
  m_boxelShader.end();
  if (!m_ray.empty()) {
    m_rayShader.begin();
    m_rayShader.setUniformTexture("textureMap", m_image.getTexture(), 1);
    m_rayRenderer->render();
    m_rayShader.end();
  }
  m_camera.end();
  //
  // 2D描画
  //
  glDisable(GL_CULL_FACE);
  ofDisableDepthTest();
  // FPSの描画
  {
    char buf[128];
    ::sprintf(buf, "%.2f", ofGetFrameRate());
    ofDrawBitmapString(buf, glm::vec2(50, 50));
  }
  // カーソル描画
  ofSetColor(255, 255, 255, 255);
  ofDrawCircle(ofGetWindowSize() / 2.0f, 5.0f);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
  if (key == 'e') {
    if (m_openInv) {
      lockMouseCursor();
    } else {
      unlockMouseCursor();
    }
    this->m_openInv = !m_openInv;
    m_controller->enabledRotation = !m_openInv;
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) { raycast(); }

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
  if (button == 0 && m_world->isContains(m_hit)) {
    m_world->setBlock(m_hit, nullptr);
    m_world->invalidate(m_hit);
    m_world->batch(*m_boxelRenderer.get());
    raycast();
  } else if (button == 2 && m_world->isContains(m_hitSide)) {
    m_world->setBlock(m_hitSide, m_stone);
    m_world->invalidate(m_hit);
    m_world->batch(*m_boxelRenderer.get());
    raycast();
  }
}

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

//--------------------------------------------------------------
void ofApp::lockMouseCursor() {
  ofAppGLFWWindow *glfwWindow = (ofAppGLFWWindow *)ofGetWindowPtr();
  glfwSetInputMode(glfwWindow->getGLFWWindow(), GLFW_CURSOR,
                   GLFW_CURSOR_HIDDEN | GLFW_CURSOR_DISABLED);
}

//--------------------------------------------------------------
void ofApp::unlockMouseCursor() {
  ofAppGLFWWindow *glfwWindow = (ofAppGLFWWindow *)ofGetWindowPtr();
  glfwSetInputMode(glfwWindow->getGLFWWindow(), GLFW_CURSOR,
                   GLFW_CURSOR_NORMAL);
}

void ofApp::raycast() {
  ofBoxel::PhysicsEngine p(1.0f);
  this->m_ray = p.ray(m_camera.getPosition(), m_camera.getLookAtDir(), 128.0f);
  bool isHit = false;
  for (auto rayPos : m_ray) {
    if (m_world->isFilled(rayPos)) {
      this->m_hit = rayPos;
      isHit = true;
      break;
    } else {
      this->m_hitSide = rayPos;
    }
  }
  if (isHit) {
    int face = static_cast<int>(p.face(m_hit, m_hitSide));
    m_rayRenderer->clear();
    m_rayRenderer->batch(m_hit, face, 0, face, 63);
  }
}