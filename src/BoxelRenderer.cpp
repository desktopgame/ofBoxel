#include "BoxelRenderer.hpp"

namespace ofBoxel {
BoxelRenderer::BoxelRenderer(ofShader shader, const ofMesh& mesh, float offset)
    : m_shader(shader),
      m_vbo(),
      m_dirty(false),
      m_attribPosition(),
      m_attribLocalOffset(),
      m_attribLocalRotation(),
      m_attribTextureSlot() {
  // 頂点情報の設定
  const auto& vertices = mesh.getVertices();
  const auto& normals = mesh.getNormals();
  const auto& index = mesh.getIndices();
  const auto& texCoords = mesh.getTexCoords();
  m_vbo.setVertexData(vertices.data(), vertices.size(), GL_STATIC_DRAW);
  m_vbo.setNormalData(normals.data(), normals.size(), GL_STATIC_DRAW);
  m_vbo.setIndexData(index.data(), index.size(), GL_STATIC_DRAW);
  m_vbo.setTexCoordData(texCoords.data(), texCoords.size(), GL_STATIC_DRAW);
  // 各種行列の作成
  m_shader.begin();
  setUniformVec3Array("localOffsetTable",
                      std::vector<glm::vec3>{
                          glm::vec3(0.0f, offset, 0.0f),   // top
                          glm::vec3(0.0f, -offset, 0.0f),  // bottom
                          glm::vec3(-offset, 0.0f, 0.0f),  // left
                          glm::vec3(offset, 0.0f, 0.0f),   // right
                          glm::vec3(0.0f, 0.0f, offset),   // front
                          glm::vec3(0.0f, 0.0f, -offset),  // back
                      });
  setUniformMatrixArray(
      "localRotationTable",
      std::vector<glm::mat4>{
          glm::rotate(glm::radians(270.0f), glm::vec3(1, 0, 0)),  // top
          glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0)),   // bottom
          glm::rotate(glm::radians(270.0f), glm::vec3(0, 1, 0)),  // left
          glm::rotate(glm::radians(90.0f), glm::vec3(0, 1, 0)),   // right
          glm::mat4(1.0f),                                        // front
          glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, 0)),  // back
      });
  m_shader.end();
}

void BoxelRenderer::batch(const glm::vec3& pos, int localOffset,
                          int localRotation, int textureSlot) {
  this->m_dirty = true;
  m_attribPosition.emplace_back(pos);
  m_attribLocalOffset.emplace_back(static_cast<float>(localOffset));
  m_attribLocalRotation.emplace_back(static_cast<float>(localRotation));
  m_attribTextureSlot.emplace_back(static_cast<float>(textureSlot));
}

void BoxelRenderer::clear() {
  m_attribPosition.clear();
  m_attribLocalOffset.clear();
  m_attribLocalRotation.clear();
  m_attribTextureSlot.clear();
  this->m_dirty = true;
}

void BoxelRenderer::rehash() {
  if (!m_dirty) {
    return;
  }
  this->m_dirty = false;
  m_vbo.setAttributeDivisor(0, 0);
  m_vbo.setAttributeDivisor(3, 0);
  // 位置を設定
  m_vbo.setAttributeData(10, &m_attribPosition.front().x, 3,
                         m_attribPosition.size(), GL_STATIC_DRAW,
                         sizeof(float) * 3);
  m_vbo.setAttributeDivisor(10, 1);
  // ローカル位置を設定
  m_vbo.setAttributeData(11, &m_attribLocalOffset.front(), 1,
                         m_attribLocalOffset.size(), GL_STATIC_DRAW,
                         sizeof(float));
  m_vbo.setAttributeDivisor(11, 1);
  // ローカル回転を設定
  m_vbo.setAttributeData(12, &m_attribLocalRotation.front(), 1,
                         m_attribLocalRotation.size(), GL_STATIC_DRAW,
                         sizeof(float));
  m_vbo.setAttributeDivisor(12, 1);
  // テクスチャを設定
  m_vbo.setAttributeData(13, &m_attribTextureSlot.front(), 1,
                         m_attribTextureSlot.size(), GL_STATIC_DRAW,
                         sizeof(float));
  m_vbo.setAttributeDivisor(13, 1);
}

void BoxelRenderer::render() {
  rehash();
  m_vbo.drawElementsInstanced(GL_TRIANGLES, 6,
                              static_cast<int>(m_attribPosition.size()));
}

// private

void BoxelRenderer::setUniformMatrixArray(const std::string& name,
                                          const std::vector<glm::mat4>& mvec) {
  GLint loc = m_shader.getUniformLocation(name);
  if (loc == -1) {
    ofLog() << name << " is not found";
    return;
  }
  std::vector<float> ptr;
  ptr.reserve(16 * mvec.size());
  for (int mp = 0; mp < 6; mp++) {
    auto const& m = mvec.at(mp);
    const float* data = glm::value_ptr(m);
    for (int i = 0; i < 16; i++) {
      ptr.emplace_back(data[i]);
    }
  }
  glUniformMatrix4fv(loc, mvec.size(), false, ptr.data());
}

void BoxelRenderer::setUniformVec3Array(const std::string& name,
                                        const std::vector<glm::vec3>& vvec) {
  m_shader.setUniform3fv(name, &vvec.front().x, vvec.size());
}
}  // namespace ofBoxel