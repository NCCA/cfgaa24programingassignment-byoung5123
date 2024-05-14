#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include "Tetromino.h"
#include "ngl/Vec4.h"

/// @struct BlockInfo
/// @brief Holds information about each block on the game board including occupancy and color.
struct BlockInfo {
    bool occupied; ///< Flag indicating whether the block is occupied.
    ngl::Vec4 colour; ///< Color of the block, represented using ngl::Vec4.
};

/// @class Board
/// @brief Manages the game board for a Tetris game, including block positions and interactions.
class Board{
public:
    /// Default constructor.
    Board() = default;

    /// Constructor to create a board with specified dimensions.
    /// @param width The width of the board.
    /// @param height The height of the board.
    Board(int width, int height);

    /// Retrieves the color of the block at the specified position.
    /// @param row The row index of the block.
    /// @param col The column index of the block.
    /// @return The color of the block if occupied; otherwise, returns transparent color.
    ngl::Vec4 GetBlock(int row, int col) const;

    /// Checks for collisions with the Tetromino.
    /// @param tetromino The tetromino to check.
    /// @param Down The downward movement to apply.
    /// @param Left The leftward movement to apply.
    /// @param Right The rightward movement to apply.
    /// @return True if there is a collision; otherwise, false.
    bool IsCollision(const Tetromino& tetromino, int Down, int Left, int Right);

    /// Clears the Tetromino from the board.
    /// @param tetromino The tetromino to clear.
    void ClearTetromino(const Tetromino& tetromino);

    /// Updates the board with the current position of the Tetromino.
    /// @param tetromino The tetromino to update on the board.
    void UpdateTetrominoOnBoard(const Tetromino& tetromino);

    /// Moves the Tetromino in the specified direction.
    /// @param tetromino The tetromino to move.
    /// @param direction The direction to move the tetromino.
    /// @return True if the move was successful; otherwise, false.
    bool MoveTetromino(Tetromino& tetromino, int direction);

    /// Rotates the Tetromino on the board.
    /// @param tetromino The tetromino to rotate.
    /// @return True if the rotation was successful; otherwise, false.
    bool RotateTetromino(Tetromino& tetromino);

    /// Clears any full rows on the board and updates the score accordingly.
    void ClearFullRows();

    /// Gets the height of the board.
    /// @return The height of the board.
    int getHeight() const;

    /// Gets the width of the board.
    /// @return The width of the board.
    int getWidth() const;

    /// Gets the current score.
    /// @return The score.
    int getScore() const;

private:
    int width_;  ///< Width of the board.
    int height_; ///< Height of the board.
    int _score = 0; ///< Current score.

    /// 2D vector holding block information for each position on the board.
    std::vector<std::vector<BlockInfo>> _blockInfo;
};

#endif // BOARD_H
