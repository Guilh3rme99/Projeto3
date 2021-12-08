#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w)
      m_dollySpeed = 4.0f;
    if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)
      m_dollySpeed = -4.0f;
    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a)
      m_panSpeed = -4.0f;
    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d)
      m_panSpeed = 4.0f;
    if (ev.key.keysym.sym == SDLK_q) m_truckSpeed = -4.0f;
    if (ev.key.keysym.sym == SDLK_e) m_truckSpeed = 4.0f;
  }
  if (ev.type == SDL_KEYUP) {
    if ((ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_q && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_e && m_truckSpeed > 0) m_truckSpeed = 0.0f;
  }
}
// IMPORTANTE = texture(skyTex, fragTexCoord)

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program(shader da textura para o metal gear)
  m_program = createProgramFromFile(getAssetsPath() + "shaders/texture.vert",
                                    getAssetsPath() + "shaders/texture.frag");
  // Create program(shader do skybox)
  g_program = createProgramFromFile(getAssetsPath() + "shaders/skybox.vert",
                                    getAssetsPath() + "shaders/skybox.frag");
  // Create program(shader blinnphong para o porta-aviões)
  s_program = createProgramFromFile(getAssetsPath() + "shaders/blinnphong.vert",
                                    getAssetsPath() + "shaders/blinnphong.frag");
                            

  //Load nos dois modelos
  loadRex(getAssetsPath() + "REX_2.obj");
  m_mappingMode = 3;  // "From mesh" option
  m_ship.loadFromFile(getAssetsPath() + "essex_scb-125_generic.obj");
  //Inicializando o ship e o skybox
  m_ship.initializeGL(s_program);
  m_sky.initializeSkybox(g_program);


  //Load da textura
  m_sky.loadCubeTexture(getAssetsPath() + "fotos/");

  resizeGL(getWindowSettings().width, getWindowSettings().height);
}
//Textura do Metal Gear
void OpenGLWindow::loadRex(std::string_view path) {
  m_rex.terminateGL();

  m_rex.loadDiffuseTexture(getAssetsPath() + "maps/Metal006_2K_Roughness.jpg");
  m_rex.loadObj(path);
  m_rex.initializeGL(m_program);

  // Use material properties from the loaded model
  m_Ka = m_rex.getKa();
  m_Kd = m_rex.getKd();
  m_Ks = m_rex.getKs();
  m_shininess = m_rex.getShininess();
}
void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);
  //draw skybox
  m_sky.renderSkybox(m_camera.m_viewMatrix,m_camera.m_projMatrix);

  // Use currently selected program
  abcg::glUseProgram(m_program);

  // Get location of uniform variables
  const GLint viewMatrixLoc{abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{abcg::glGetUniformLocation(m_program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  /*const GLint normalMatrixLoc{
      abcg::glGetUniformLocation(m_program, "normalMatrix")};*/
  const GLint lightDirLoc{
      abcg::glGetUniformLocation(m_program, "lightDirWorldSpace")};
  const GLint shininessLoc{abcg::glGetUniformLocation(m_program, "shininess")};
  const GLint IaLoc{abcg::glGetUniformLocation(m_program, "Ia")};
  const GLint IdLoc{abcg::glGetUniformLocation(m_program, "Id")};
  const GLint IsLoc{abcg::glGetUniformLocation(m_program, "Is")};
  const GLint KaLoc{abcg::glGetUniformLocation(m_program, "Ka")};
  const GLint KdLoc{abcg::glGetUniformLocation(m_program, "Kd")};
  const GLint KsLoc{abcg::glGetUniformLocation(m_program, "Ks")};
  const GLint diffuseTexLoc{abcg::glGetUniformLocation(m_program, "diffuseTex")};
  const GLint mappingModeLoc{
      abcg::glGetUniformLocation(m_program, "mappingMode")};
 

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);

  const auto lightDirRotated{m_lightDir};
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform1f(shininessLoc, m_shininess);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  // Set uniform variables of the current object
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  
  //draw rex
  m_rex.paintGL(m_camera.m_viewMatrix);

  // Using another program
  abcg::glUseProgram(s_program);

    // Get location of uniform variables
  const GLint viewMatrixLoc2{abcg::glGetUniformLocation(s_program, "viewMatrix")};
  const GLint projMatrixLoc2{abcg::glGetUniformLocation(s_program, "projMatrix")};
  const GLint modelMatrixLoc2{
      abcg::glGetUniformLocation(s_program, "modelMatrix")};
 /* const GLint normalMatrixLoc{
      abcg::glGetUniformLocation(s_program, "normalMatrix")};*/
  const GLint lightDirLoc2{
      abcg::glGetUniformLocation(s_program, "lightDirWorldSpace")};
  const GLint shininessLoc2{abcg::glGetUniformLocation(s_program, "shininess")};
  const GLint IaLoc2{abcg::glGetUniformLocation(s_program, "Ia")};
  const GLint IdLoc2{abcg::glGetUniformLocation(s_program, "Id")};
  const GLint IsLoc2{abcg::glGetUniformLocation(s_program, "Is")};
  const GLint KaLoc2{abcg::glGetUniformLocation(s_program, "Ka")};
  const GLint KdLoc2{abcg::glGetUniformLocation(s_program, "Kd")};
  const GLint KsLoc2{abcg::glGetUniformLocation(s_program, "Ks")};

  abcg::glUniformMatrix4fv(viewMatrixLoc2, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc2, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);

  const auto lightDirRotated2{m_lightDir};
  abcg::glUniform4fv(lightDirLoc2, 1, &lightDirRotated.x);
  abcg::glUniform1f(shininessLoc2, m_shininess);
  abcg::glUniform4fv(IaLoc2, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc2, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc2, 1, &m_Is.x);
  abcg::glUniform4fv(KaLoc2, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc2, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc2, 1, &m_Ks.x);

  abcg::glUniformMatrix4fv(modelMatrixLoc2, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  //draw ship
  m_ship.paintGL(m_camera.m_viewMatrix);

  abcg::glUseProgram(0);
}
void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  m_rex.terminateGL();
  m_ship.terminateGL();
  m_sky.terminateSkybox();
  
}
void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}