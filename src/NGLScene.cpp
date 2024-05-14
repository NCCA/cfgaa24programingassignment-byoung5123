#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <QGuiApplication>
#include <QMouseEvent>
#include "Cube.h"
#include "Tetromino.h"
#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <QPainter>

NGLScene::NGLScene()
{
  setTitle("nglTetris");
}

NGLScene::~NGLScene()
{
  std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(int _w, int _h)
{
  m_projection = ngl::perspective(45.0f, static_cast<float>(_w) / _h, 0.1f, 200.0f);

  m_win.width = static_cast<int>(_w * devicePixelRatio());
  m_win.height = static_cast<int>(_h * devicePixelRatio());
}
constexpr auto shaderProgram = "PBR";

void NGLScene::initializeGL()
{
  // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::initialize();
  // uncomment this line to make ngl less noisy with debug info
  // ngl::NGLInit::instance()->setCommunicationMode( ngl::CommunicationMode::NULLCONSUMER);
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f); // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  // now to load the shader and set the values
  // grab an instance of shader manager
  // in the code create some constexpr
  constexpr auto vertexShader = "PBRVertex";
  constexpr auto fragShader = "PBRFragment";
  // create the shader program
  ngl::ShaderLib::createShaderProgram(shaderProgram);
  // now we are going to create empty shaders for Frag and Vert
  ngl::ShaderLib::attachShader(vertexShader, ngl::ShaderType::VERTEX);
  ngl::ShaderLib::attachShader(fragShader, ngl::ShaderType::FRAGMENT);
  // attach the source
  ngl::ShaderLib::loadShaderSource(vertexShader, "shaders/PBRVertex.glsl");
  ngl::ShaderLib::loadShaderSource(fragShader, "shaders/PBRFragment.glsl");
  // compile the shaders
  ngl::ShaderLib::compileShader(vertexShader);
  ngl::ShaderLib::compileShader(fragShader);
  // add them to the program
  ngl::ShaderLib::attachShaderToProgram(shaderProgram, vertexShader);
  ngl::ShaderLib::attachShaderToProgram(shaderProgram, fragShader);
  // now we have associated that data we can link the shader
  ngl::ShaderLib::linkProgramObject(shaderProgram);
  // and make it active ready to load values
  ngl::ShaderLib::use(shaderProgram);
  // We now create our view matrix for a static camera
  ngl::Vec3 from{0.0f, 15.0f, 20.0f};
  ngl::Vec3 to{0.0f, 8.0f, 0.0f};
  ngl::Vec3 up{0.0f, 1.0f, 0.0f};
  // now load to our new camera
  m_view = ngl::lookAt(from, to, up);
  ngl::ShaderLib::setUniform("camPos", from);
  // now a light
  m_lightPos.set(5.0, 5.0f, 30.0f, 1.0f);
  m_lightAngle = 0.0f;
  // setup the default shader material and light porerties
  // these are "uniform" so will retain their values
  ngl::ShaderLib::setUniform("lightPosition", m_lightPos.toVec3());
  ngl::ShaderLib::setUniform("lightColor", 1600.0f, 1600.0f, 1600.0f);
  ngl::ShaderLib::setUniform("exposure", 6.0f);
  ngl::ShaderLib::setUniform("albedo", 0.950f, 0.71f, 0.29f);

  ngl::ShaderLib::setUniform("metallic", 1.02f);
  ngl::ShaderLib::setUniform("roughness", 0.38f);
  ngl::ShaderLib::setUniform("ao", 0.2f);
  ngl::VAOPrimitives::createTrianglePlane("floor", 20, 20, 1, 1, ngl::Vec3::up());
  ngl::ShaderLib::printRegisteredUniforms(shaderProgram);
  ngl::ShaderLib::use(ngl::nglCheckerShader);
  ngl::ShaderLib::setUniform("lightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
  ngl::ShaderLib::setUniform("checkOn", true);
  ngl::ShaderLib::setUniform("lightPos", m_lightPos.toVec3());
  ngl::ShaderLib::setUniform("colour1", 0.9f, 0.9f, 0.9f, 1.0f);
  ngl::ShaderLib::setUniform("colour2", 0.6f, 0.6f, 0.6f, 1.0f);
  ngl::ShaderLib::setUniform("checkSize", 60.0f);
  ngl::ShaderLib::printRegisteredUniforms(ngl::nglCheckerShader);

    // Set up the timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(gameLoopTick()));
    m_timer.start(300); // Start the timer with a 300ms interval

    // Seed the random number generator with the current time
    srand(time(NULL));
}

void NGLScene::updateCubes()
{
    // update cubes
    m_cubes.clear();
    for (int row = 0; row < m_board.getHeight(); ++row)
    {
        for (int col = 0; col < m_board.getWidth(); ++col)
        {
            if (m_board.GetBlock(row, col) != ngl::Vec4 {0,0,0,0})
            {
                // Calculate the position of the cube based on row and column
                auto posX = static_cast<float>(col);// Adjust cubeSize as needed
                auto posY = static_cast<float>(row); // Adjust cubeSize as needed
                float posZ = 0.0f; // Adjust if you want the cubes to be in different planes

                // Create a cube at the calculated position
                ngl::Vec3 pos = {posX, posY, posZ};
                ngl::Vec4 colour = m_board.GetBlock(row, col); // Green color for blocks
                Cube blockCube(pos, colour);

                // Add the cube to the vector in NGLScene
                AddCube(blockCube);
                update();
            }
        }
    }
}

void NGLScene::gameLoopTick()
{
    // game loop //
    if(m_board.MoveTetromino(m_tetromino, 1))
    { // move tetromino down and check for collision
        //std::cout << "collision";
        m_board.UpdateTetrominoOnBoard(m_tetromino);
        m_board.ClearFullRows();
        std::cout<<m_board.getScore();
        m_tetromino = Tetromino(rand() % 7 + 1, 4, 16); // Spawn new Tetromino
    }
    updateCubes();
}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib::use("PBR");
  struct transform
  {
    ngl::Mat4 MVP;
    ngl::Mat4 normalMatrix;
    ngl::Mat4 M;
  };

  transform t;
  t.M = m_view * m_mouseGlobalTX;

  t.MVP = m_projection * t.M;
  t.normalMatrix = t.M;
  t.normalMatrix.inverse().transpose();
  // ngl::msg->addMessage(fmt::format("size {0}",sizeof(transform)));
  if (m_transformLight)
  {
    ngl::ShaderLib::setUniform("lightPosition", (m_mouseGlobalTX * m_lightPos).toVec3());
  }
}

void NGLScene::AddCube(const Cube& cube)
{
    m_cubes.push_back(cube);
}

void NGLScene::setTetromino(const Tetromino& tetromino)
{
    m_tetromino = tetromino;
}

void NGLScene::setBoard(const Board& board)
{
    m_board = board;
}

void NGLScene::paintGL()
{
  glViewport(0, 0, m_win.width, m_win.height);
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // grab an instance of the shader manager
  ngl::ShaderLib::use("PBR");

  // Rotation based on the mouse position for our global transform
  auto rotX = ngl::Mat4::rotateX(m_win.spinXFace);
  auto rotY = ngl::Mat4::rotateY(m_win.spinYFace);

  // multiply the rotations
  m_mouseGlobalTX = rotX * rotY;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;
  // draw
  loadMatricesToShader();

  ngl::ShaderLib::use("PBR");
  // draw tetris cubes
  for (auto& cube : m_cubes)
  {
        cube.updatePos();
        cube.draw("PBR", m_mouseGlobalTX, m_view, m_projection);
  }

  ngl::ShaderLib::use(ngl::nglCheckerShader);
  auto tx = ngl::Mat4::translate(0.0f, -0.6f, 0.0f);
  auto ts = ngl::Mat4::scale(0.5f, 0.5f, 0.25f);
  auto MVP = m_projection * m_view * m_mouseGlobalTX * tx * ts;
  ngl::Mat3 normalMatrix = m_view * m_mouseGlobalTX;
  normalMatrix.inverse().transpose();
  ngl::ShaderLib::setUniform("MVP", MVP);
  ngl::ShaderLib::setUniform("normalMatrix", normalMatrix);
  if (m_transformLight)
  {
    ngl::ShaderLib::setUniform("lightPosition", (m_mouseGlobalTX * m_lightPos).toVec3());
  }
  ngl::VAOPrimitives::draw("floor");

}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // that method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quit
  case Qt::Key_Escape:
    QGuiApplication::exit(EXIT_SUCCESS);
    break;
// turn on wireframe rendering
#ifndef USINGIOS_
  case Qt::Key_W:
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    break;
  // turn off wire frame
  case Qt::Key_S:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
#endif
    // Game controls //
  case Qt::Key_Down:
      m_board.MoveTetromino(m_tetromino, 1);
      updateCubes();
      break;
  case Qt::Key_Left:
    m_board.MoveTetromino(m_tetromino, 2);
      updateCubes();
    break;
  case Qt::Key_Right:
      m_board.MoveTetromino(m_tetromino, 3);
      updateCubes();
      break;
  case Qt::Key_Up:
      m_board.RotateTetromino(m_tetromino);
      updateCubes();
      break;
    //              //
  // show full screen
  case Qt::Key_F:
    showFullScreen();
    break;
  // show windowed
  case Qt::Key_N:
    showNormal();
    break;
  case Qt::Key_Space:
    m_win.spinXFace = 0;
    m_win.spinYFace = 0;
    m_modelPos.set(ngl::Vec3::zero());
    break;
  case Qt::Key_L:
    m_transformLight ^= true;
    break;
  default:
    break;
  }
  update();
}
