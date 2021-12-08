#ifndef SHIP_HPP_
#define SHIP_HPP_

#include <vector>

#include "abcg.hpp"
#include "util.hpp"


class Ship {
 public:
  void initializeGL(GLuint program);
  void paintGL(glm::mat4 m_viewMatrix);
  void terminateGL();
  void loadFromFile(std::string_view path, bool standardize = true);
  

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint normalMatrixShip{};
  GLuint modelMatrixShip{};
  glm::mat4 modelMatrix{1.0f};
  GLuint m_program{};
  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;
  GLint C;

  void update();

  bool m_hasNormals{false};

  void computeNormals();
  void standardize();
};

#endif