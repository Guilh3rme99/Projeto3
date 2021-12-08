#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcg.hpp"
#include "util.hpp"

class Ground {
 public:
  void initializeGL(GLuint program);
  void paintGL(glm::mat4 m_viewMatrix,glm::mat4 m_projMatrix);
  void terminateGL();

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint ground_program{};
  GLuint textureID{};

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};

  // Unit quad on the xz plane
  // clang-format off
  std::array <glm::vec3, 6> vertices{    
    glm::vec3{-1, -1, -1}, glm::vec3{+1, -1, -1}, glm::vec3{+1, -1, +1},
    glm::vec3{-1, -1, -1}, glm::vec3{+1, -1, +1}, glm::vec3{-1, -1, +1}};

};

#endif