/****************************************************************************
OpenGL Tetris game modified from http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
****************************************************************************/
#include "NGLScene.h"
#include <QtGui/QGuiApplication>
#include <iostream>
#include "Board.h"
#include "Tetromino.h"

int main(int argc, char** argv) {
    // Create a GUI application
    QGuiApplication app(argc, argv);

    // Create an OpenGL format specifier
    QSurfaceFormat format;

    // Set the number of samples for multisampling
    // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
    format.setSamples(4);

    // Define the OpenGL context version
#if defined(__APPLE__)
    // Mac OS X supports OpenGL version 4.1 under MacOS Mojave and earlier
    format.setMajorVersion(4);
    format.setMinorVersion(1);
#else
    // Assume the latest OpenGL version if not on Mac OS X
    format.setMajorVersion(4);
    format.setMinorVersion(5);
#endif

    // Set to CoreProfile to use modern OpenGL features
    format.setProfile(QSurfaceFormat::CoreProfile);

    // Set the depth buffer to 24 bits
    format.setDepthBufferSize(24);

    // Apply the configured format as the default for all windows
    QSurfaceFormat::setDefaultFormat(format);

    // Create the main window using NGLScene, which inherits from QOpenGLWindow
    NGLScene window;

    // Query and print the OpenGL version used
    std::cout << "Profile is " << format.majorVersion() << "." << format.minorVersion() << "\n";

    // Set the initial window size
    window.resize(720, 1024);

    // Tetris-specific setup
    int startx = 4; // Initial x-coordinate for Tetromino (adjusted to leave space)
    int starty = 16; // Initial y-coordinate for Tetromino

    // Initialize the game board and a tetromino piece
    Board board(11, 20);
    Tetromino tetromino(6, startx, starty);
    window.setTetromino(tetromino);
    window.setBoard(board);

    // Display the window
    window.show();

    // Run the application loop
    return app.exec();
}

