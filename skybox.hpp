#ifndef SKYBOX_HPP_
#define SKYBOX_HPP_

#include <vector>

#include "abcg.hpp"
#include "util.hpp"


class Sky {
 public:
  void initializeSkybox(GLuint program);
  void renderSkybox(glm::mat4 m_viewMatrix,glm::mat4 m_projMatrix);
  void terminateSkybox();
  void loadCubeTexture(const std::string &Tpath);

 

 private:
  // Componentes do skybox
  const std::array<glm::vec3, 36> m_skyPositions{
       // Front
    glm::vec3{-1, -1, +1}, glm::vec3{+1, -1, +1}, glm::vec3{+1, +1, +1},
    glm::vec3{-1, -1, +1}, glm::vec3{+1, +1, +1}, glm::vec3{-1, +1, +1},
    // Back
    glm::vec3{+1, -1, -1}, glm::vec3{-1, -1, -1}, glm::vec3{-1, +1, -1},
    glm::vec3{+1, -1, -1}, glm::vec3{-1, +1, -1}, glm::vec3{+1, +1, -1},
    // Right
    glm::vec3{+1, -1, -1}, glm::vec3{+1, +1, -1}, glm::vec3{+1, +1, +1},
    glm::vec3{+1, -1, -1}, glm::vec3{+1, +1, +1}, glm::vec3{+1, -1, +1},
    // Left
    glm::vec3{-1, -1, +1}, glm::vec3{-1, +1, +1}, glm::vec3{-1, +1, -1},
    glm::vec3{-1, -1, +1}, glm::vec3{-1, +1, -1}, glm::vec3{-1, -1, -1},
    // Top
    glm::vec3{-1, +1, +1}, glm::vec3{+1, +1, +1}, glm::vec3{+1, +1, -1},
    glm::vec3{-1, +1, +1}, glm::vec3{+1, +1, -1}, glm::vec3{-1, +1, -1},
    // Bottom
    glm::vec3{-1, -1, -1}, glm::vec3{+1, -1, -1}, glm::vec3{+1, -1, +1},
    glm::vec3{-1, -1, -1}, glm::vec3{+1, -1, +1}, glm::vec3{-1, -1, +1}
  };
  
  GLuint m_skyProgram{};
  GLuint m_skyVAO{};
  GLuint m_skyVBO{};
  GLuint cubeTexture{};

};
#endif