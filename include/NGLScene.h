#ifndef NGLSCENE_H_
#define NGLSCENE_H_

#include <ngl/Text.h>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/Mat4.h>
#include "WindowParams.h"
#include <QOpenGLWindow>
#include <QTimer>
#include "Cube.h"
#include "Tetromino.h"
#include "Board.h"

//----------------------------------------------------------------------------------------------------------------------
/// @class NGLScene
/// @brief This class inherits from QOpenGLWindow and utilizes the NGL library to manage and render OpenGL scenes.
/// It acts as the main drawing class where all OpenGL elements are controlled.
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWindow
{
Q_OBJECT

public:
    /// Constructor for the NGL drawing class
    NGLScene();

    /// Destructor - cleans up NGL and releases OpenGL resources
    virtual ~NGLScene();

    /// Initialize OpenGL settings and prepare any required resources
    void initializeGL() override;

    /// Render the OpenGL scene
    void paintGL() override;

    /// Handle OpenGL window resizing
    void resizeGL(int _width, int _height) override;

    /// Add a cube to the scene
    void AddCube(const Cube& cube);

    /// Set the current Tetromino
    void setTetromino(const Tetromino& tetromino);

    /// Set the game board
    void setBoard(const Board& board);

    /// Update cubes based on game state changes
    void updateCubes();

private slots:
    /// Slot to handle game loop ticks
    void gameLoopTick();

private:
    WinParams m_win;                ///< Window parameters such as mouse controls and rotation settings
    ngl::Mat4 m_mouseGlobalTX;      ///< Global transformation for mouse interaction
    ngl::Vec3 m_modelPos;           ///< Position of the model for mouse interaction
    ngl::Mat4 m_view;               ///< View matrix for the camera
    ngl::Mat4 m_projection;         ///< Projection matrix for the camera
    bool m_transformLight = false;  ///< Flag to determine if the light should be transformed
    ngl::Vec4 m_lightPos;           ///< Position of the light in the scene
    ngl::Real m_lightAngle;         ///< Angle of the light

    /// Load transformation matrices into the shader program
    void loadMatricesToShader();

    /// Handle key press events
    void keyPressEvent(QKeyEvent* _event) override;

    /// Handle mouse movement events
    void mouseMoveEvent(QMouseEvent* _event) override;

    /// Handle mouse button press events
    void mousePressEvent(QMouseEvent* _event) override;

    /// Handle mouse button release events
    void mouseReleaseEvent(QMouseEvent* _event) override;

    /// Handle mouse wheel events
    void wheelEvent(QWheelEvent* _event) override;

    std::vector<Cube> m_cubes;      ///< Vector of cubes representing parts of the Tetris game
    Tetromino m_tetromino;          ///< Current Tetromino in play
    Board m_board;                  ///< Game board
    QTimer m_timer;                 ///< Timer for game loop ticks
};

#endif // NGLSCENE_H_
