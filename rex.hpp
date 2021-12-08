#ifndef REX_HPP_
#define REX_HPP_

#include <vector>

#include "abcg.hpp"
#include "util.hpp"


class Rex {
 public:
  void initializeGL(GLuint program);
  void paintGL(glm::mat4 m_viewMatrix);
  void terminateGL();
  void loadDiffuseTexture(std::string_view path);
  void loadObj(std::string_view path, bool standardize = true);

  [[nodiscard]] glm::vec4 getKa() const { return m_Ka; }
  [[nodiscard]] glm::vec4 getKd() const { return m_Kd; }
  [[nodiscard]] glm::vec4 getKs() const { return m_Ks; }
  [[nodiscard]] float getShininess() const { return m_shininess; }

  [[nodiscard]] bool isUVMapped() const { return m_hasTexCoords; }

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint normalMatrixRex{};
  GLuint modelMatrixRex{};
  
  GLuint m_program{};

  glm::vec4 m_Ka;
  glm::vec4 m_Kd;
  glm::vec4 m_Ks;
  float m_shininess;
  GLuint m_diffuseTexture{};

  bool m_hasNormals{false};
  bool m_hasTexCoords{false};


  //Usamos um array que guarda duas matrizes modelo para diferentes posições
  std::array<glm::mat4,2> modelMatrix;
  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void update();

  void computeNormals();
  void standardize();
};

#endif