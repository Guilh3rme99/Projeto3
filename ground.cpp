#include "ground.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <imgui.h>
#include <fmt/core.h>
#include <tiny_obj_loader.h>

void Ground::initializeGL(GLuint program) {
  //Grab wanted texture
  glGenTextures(1, &textureID);
  textureID = abcg::opengl::loadTexture("blue-water-with-ripples-texture.jpg");
  ground_program = program;
  
  
  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                     vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  const GLint positionAttribute{
      abcg::glGetAttribLocation(ground_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Ground::paintGL(glm::mat4 m_viewMatrix,glm::mat4 m_projMatrix) {

  abcg::glUseProgram(ground_program);
  printf("Foi ?");
  // Get location of uniform variables
  const GLint viewMatrixLoc{
      abcg::glGetUniformLocation(ground_program, "viewMatrix")};
  const GLint projMatrixLoc{
      abcg::glGetUniformLocation(ground_program, "projMatrix")};
  const GLint skyTexLoc{abcg::glGetUniformLocation(ground_program, "skyTex")};

  // Set uniform variables
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(skyTexLoc, 0);

  abcg::glBindVertexArray(m_VAO);

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  abcg::glEnable(GL_CULL_FACE);
  abcg::glFrontFace(GL_CW);
  abcg::glDepthFunc(GL_LEQUAL);
  abcg::glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  abcg::glDepthFunc(GL_LESS);

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}

void Ground::terminateGL() {
  abcg::glDeleteTextures(1, &textureID);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}