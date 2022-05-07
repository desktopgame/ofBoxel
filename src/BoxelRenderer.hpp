#pragma once
#include <ofMesh.h>
#include <ofShader.h>
#include <ofVbo.h>
#include <ofVectorMath.h>

namespace ofBoxel {
class BoxelRenderer {
 public:
  explicit BoxelRenderer(ofShader shader, const ofMesh& mesh,
                         float offset = 0.5f);
  void clear();
  void batch(const glm::vec3& pos, int localOffset, int localRotation,
             int textureSlot);
  void compact(const std::vector<glm::ivec3>& update);
  void rehash();
  void render();

 private:
  template <typename T>
  void compact(const std::vector<int>& table, std::vector<T>& src) {
    std::vector<T> tmp;
    tmp.resize(src.size());
    // 変更してはいけない領域を詰めながら追加
    int index = 0;
    int offset = 0;
    for (int next : table) {
      if (index == next) {
        index = next + 1;
        continue;
      }
      std::copy(src.begin() + index, src.begin() + next, tmp.begin() + offset);
      offset += next - index;
      index = next + 1;
    }
    if (index < src.size()) {
      std::copy(src.begin() + index, src.end(), tmp.begin() + offset);
      offset += src.size() - index;
    }
    // 最後に変更してもいい領域を追加
    for (int i = 0; i < table.size(); i++) {
      tmp.at(offset + i) = src.at(table.at(i));
    }
    // 本体にコピペ
    src.swap(tmp);
  }
  void setUniformMatrixArray(const std::string& name,
                             const std::vector<glm::mat4>& mvec);
  void setUniformVec3Array(const std::string& name,
                           const std::vector<glm::vec3>& vvec);
  ofShader m_shader;
  ofVbo m_vbo;
  bool m_dirty;
  int m_freeIndex;
  std::vector<glm::vec3> m_attribPosition;
  std::vector<float> m_attribLocalOffset;
  std::vector<float> m_attribLocalRotation;
  std::vector<float> m_attribTextureSlot;
};
}  // namespace ofBoxel