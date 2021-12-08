#include "skybox.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>
#include <abcg_openglwindow.hpp>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include <filesystem>

void Sky::initializeSkybox(GLuint program) {
  
  // Create skybox program
  m_skyProgram = program;
 

  // Generate VBO
  abcg::glGenBuffers(1, &m_skyVBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyPositions),
                     m_skyPositions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  const GLint positionAttribute{
      abcg::glGetAttribLocation(m_skyProgram, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_skyVAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_skyVAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Sky::loadCubeTexture(const std::string& Tpath) {
  if (!std::filesystem::exists(Tpath)) return;
  
  abcg::glDeleteTextures(1, &cubeTexture);
  cubeTexture = abcg::opengl::loadCubemap(
      {Tpath + "px.png", Tpath + "nx.png", Tpath + "py.png",
       Tpath + "ny.png", Tpath + "pz.png", Tpath + "nz.png"});
}

void Sky::renderSkybox(glm::mat4 m_viewMatrix,glm::mat4 m_projMatrix) {
  abcg::glUseProgram(m_skyProgram);

  // Get location of uniform variables
  const GLint viewMatrixSky{
      abcg::glGetUniformLocation(m_skyProgram, "viewMatrix")};
  const GLint projMatrixSky{
      abcg::glGetUniformLocation(m_skyProgram, "projMatrix")};
  const GLint skyTexLoc{abcg::glGetUniformLocation(m_skyProgram, "skyTex")};

  // Set uniform variables
  abcg::glUniformMatrix4fv(viewMatrixSky, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixSky, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(skyTexLoc, 0);

  abcg::glBindVertexArray(m_skyVAO);
  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);


  abcg::glEnable(GL_CULL_FACE);
  abcg::glFrontFace(GL_CW);
  abcg::glDepthFunc(GL_LEQUAL);
  abcg::glDrawArrays(GL_TRIANGLES, 0, m_skyPositions.size());
  abcg::glDepthFunc(GL_LESS);
  abcg::glFrontFace(GL_CCW);

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}

void Sky::terminateSkybox() {
  abcg::glDeleteBuffers(1, &m_skyVBO);
  abcg::glDeleteVertexArrays(1, &m_skyVAO);
  abcg::glDeleteProgram(m_skyProgram);
}