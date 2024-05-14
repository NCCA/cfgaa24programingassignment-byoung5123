#include "Tetromino.h"
#include "ngl/Vec4.h"
#include <iostream>

// Constructor for initializing a Tetromino with type, x, and y positions.
Tetromino::Tetromino(int type, int x, int y) : _type(type), _x(x), _y(y)
{
    initializeShape();
    initializeColor();
    SetPosition(x, y);
}

// Initializes the Tetromino's shape based on its type and current rotation state.
void Tetromino::initializeShape()
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            _shape[i][j] = rotationStates[_type-1][_currentRotationState][i][j];
        }
    }
}

// Initializes the Tetromino's color based on its type.
void Tetromino::initializeColor()
{
    static const std::array<ngl::Vec4, 7> colours =
            {
            ngl::Vec4(0.0f, 0.0f, 1.0f, 1.0f),   // I-block
            ngl::Vec4(1.0f, 0.0f, 1.0f, 1.0f), // T-block
            ngl::Vec4(0.5f, 0.0f, 1.0f, 1.0f),   // O-block
            ngl::Vec4(0.0f, 1.0f, 0.0f, 1.0f),   // Z-block
            ngl::Vec4(1.0f, 0.0f, 0.0f, 1.0f),   // S-block
            ngl::Vec4(1.0f, 1.0f, 0.0f, 1.0f), // L-block
            ngl::Vec4(0.0f, 1.0f, 1.0f, 1.0f) // J-block
            };
    _colour = colours[_type - 1];
}

// Rotates the Tetromino to the next orientation state.
void Tetromino::Rotate()
{
    _currentRotationState = (_currentRotationState + 1) % 4;
    initializeShape(); // Reinitialize the shape for the new rotation state
    //std::cout << "New rotation state: " << _currentRotationState << std::endl;
}

// Returns the color of the Tetromino.
ngl::Vec4 Tetromino::getColour() const
{
    return _colour;
}

// Returns the block type at the specified row and column of the shape.
int Tetromino::getBlock(int row, int col) const
{
    return _shape[row][col];
}

// Returns the x-coordinate of the Tetromino's position.
int Tetromino::GetX() const
{
    return _x;
}

// Returns the y-coordinate of the Tetromino's position.
int Tetromino::GetY() const
{
    return _y;
}

// Sets the position of the Tetromino.
void Tetromino::SetPosition(int x, int y)
{
    _x = x;
    _y = y;
    //std::cout << "Tetromino position set to (" << x << ", " << y << ")" << std::endl;
}

bool Tetromino::IsWithinShape(int i, int j) const
{
    // Ensure the indices are within the bounds of the shape array
    if (i >= 0 && i < 4 && j >= 0 && j < 4)
    {
        return _shape[i][j] == 1;
    }
    return false;
}