#pragma once
#include <ofMesh.h>
#include <ofShader.h>
#include <ofVbo.h>

namespace ofBoxel {
class BoxelRenderer {
 public:
  explicit BoxelRenderer(ofShader shader, const ofMesh& mesh,
                         float offset = 0.5f);
  void clear();
  void batch(const glm::vec3& pos, int localOffset, int localRotation,
             int textureSlot);
  void rehash();
  void render();

 private:
  void setUniformMatrixArray(const std::string& name,
                             const std::vector<glm::mat4>& mvec);
  void setUniformVec3Array(const std::string& name,
                           const std::vector<glm::vec3>& vvec);
  ofShader m_shader;
  ofVbo m_vbo;
  bool m_dirty;
  std::vector<glm::vec3> m_attribPosition;
  std::vector<float> m_attribLocalOffset;
  std::vector<float> m_attribLocalRotation;
  std::vector<float> m_attribTextureSlot;
};
}  // namespace ofBoxel