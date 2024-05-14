#ifndef TETROMINO_H
#define TETROMINO_H

#include <array>
#include "ngl/Vec4.h"

/// @class Tetromino
/// @brief Manages the properties and behavior of Tetromino blocks in a Tetris game.
///
/// Provides functionality for managing different types of Tetromino blocks,
/// including their rotation, position, and color. Blocks are defined within a 4x4 grid.
class Tetromino
{
public:
    /// Default constructor.
    Tetromino() = default;

    /// Constructor to initialize a Tetromino with a specific type and position.
    /// @param type The type of the tetromino.
    /// @param x The x-coordinate on the game board.
    /// @param y The y-coordinate on the game board.
    Tetromino(int type, int x, int y);

    /// Get the block type at the specified position within the tetromino shape.
    /// @param row The row to query.
    /// @param col The column to query.
    /// @return The block type at the given position.
    int getBlock(int row, int col) const;

    /// Get the color of this Tetromino.
    /// @return The color as an ngl::Vec4.
    ngl::Vec4 getColour() const;

    /// Get the x-coordinate of the Tetromino on the board.
    /// @return The x-coordinate.
    int GetX() const;

    /// Get the y-coordinate of the Tetromino on the board.
    /// @return The y-coordinate.
    int GetY() const;

    /// Set the position of the Tetromino.
    /// @param x The new x-coordinate on the board.
    /// @param y The new y-coordinate on the board.
    void SetPosition(int x, int y);

    /// Rotate the Tetromino to the next orientation.
    void Rotate();

    /// Check if a specific block position is within the shape of the Tetromino.
    /// @param i The row index to check.
    /// @param j The column index to check.
    /// @return True if the position is within the Tetromino's shape, false otherwise.
    bool IsWithinShape(int i, int j) const;

private:
    int _type;  ///< Stores the tetromino type (integer).
    std::array<std::array<int, 4>, 4> _shape; ///< 4x4 "shape" array to represent the tetromino shape data.
    int _x;  ///< X position on the board.
    int _y;  ///< Y position on the board.
    ngl::Vec4 _colour; ///< Colour of the Tetromino.
    int _currentRotationState = 0; ///< Index to track the current rotation state.

    /// Initializes the shape of the Tetromino based on its type and current rotation.
    void initializeShape();

    /// Initializes the color of the Tetromino based on its type.
    void initializeColor();

    /// Stores each rotation shape for each type of tetromino
    static constexpr int rotationStates[7][4][4][4] =// type, rotation, x, y
            {
            // I-Block //
            //rot0
            {{{0, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0}},
            //rot90
            {{0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
            //rot180
            {{0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0}},
            //rot270
            {{0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}}},

            // T-Block //
            //rot0
            {{{0, 0, 0, 0},
             {1, 1, 1, 0},
             {0, 1, 0, 0},
             {0, 0, 0, 0}},
            //rot90
            {{0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}},
            //rot180
            {{0, 1, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
            //rot270
            {{0, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}}},

            // O-Block //
            {{
              {1, 1, 0, 0},
             {1, 1, 0, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}},
            {
             {1, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
                    {
             {1, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
                    {
             {1, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}}},

            // Z-Block //
            {        //rot0
             {{0, 0, 0, 0},
             {0, 1, 0, 0},
             {1, 1, 0, 0},
             {1, 0, 0, 0}},
                    //rot90
            {{0, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}},
                    //rot180
            {{0, 0, 0, 0},
            {0, 0, 1, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0}},
            //rot270
            {{0, 0, 0, 0},
            {0, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 1, 0}}},

            // S-Block //
            {//rot0
             {{1, 0, 0, 0},
             {1, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 0, 0, 0}},
            //rot90
            {{0, 1, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
            //rot180
            {{0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0}},
            //rot270
            {{0, 0, 0, 0},
            {0, 1, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}}},

            // L-Block //
            {//rot0
             {{0, 1, 0, 0},
             {0, 1, 0, 0},
             {1, 1, 0, 0},
             {0, 0, 0, 0}},
            //rot90
            {{0, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0}},

            //rot180
            {{0, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}},
            //rot270
            {{1, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}}},

            // J-Block //
            { //rot0
            {{1, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}},
            //rot90
            {{0, 0, 0, 0},
            {0, 0, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}},
            //rot180
            {{1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}},
            //rot270
            {{0, 0, 0, 0},
            {1, 1, 1, 0},
            {1, 0, 0, 0},
            {0, 0, 0, 0}}},
    };
};
#endif
