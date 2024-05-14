#include "Board.h"
#include "Cube.h"
#include "ngl/Vec4.h"

Board::Board(int width, int height) : width_(width), height_(height) {
    // Initialize occupied array with all false (empty)
    _blockInfo.resize(height_, std::vector<BlockInfo>(width_, {false, ngl::Vec4()}));
}

int Board::getHeight() const
{
    return height_;
}

int Board::getWidth() const
{
    return width_;
}

int Board::getScore() const
{
    return _score;
}

bool Board::IsCollision(const Tetromino& tetromino, int Down, int Left, int Right)
{
  // Check for collisions with occupied positions or out-of-bounds movement
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      if (tetromino.getBlock(i, j) == 1)
      {
      // Calculate potential new position based on Tetromino's shape
      int newY = tetromino.GetY() + i - Down;
      int newX = tetromino.GetX() + j + Right - Left;

      // Check for out-of-bounds movement (both horizontal and vertical)
      if (newX < 0 || newX >= width_-1 || newY < 0)
      {
        return true; // Collision detected (out-of-bounds)
      }

      // Check for collision with occupied positions on board (excluding Tetromino itself)
      if (tetromino.getBlock(i, j) == 1 &&
            _blockInfo[newY][newX].occupied &&
          !tetromino.IsWithinShape(i-Down, j+Right-Left))
          { // Use unmodified IsWithinShape here
            return true; // Collision with occupied position (excluding Tetromino itself)
          }
      }
    }
  }
  // No collision detected
  return false;
}

void Board::ClearTetromino(const Tetromino& tetromino)
{
    // Iterate over Tetromino shape and clear positions from the board
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (tetromino.getBlock(i, j) == 1)
            {
                int newY = tetromino.GetY() + i;
                int newX = tetromino.GetX() + j;
                if (newY < height_ && newX < width_)
                {
                    _blockInfo[newY][newX].occupied = false;  // Clear occupied state
                    _blockInfo[newY][newX].colour = ngl::Vec4{0,0,0,0};
                }
            }
        }
    }
}

void Board::UpdateTetrominoOnBoard(const Tetromino& tetromino)
{
    // Update board with new Tetromino positions
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (tetromino.getBlock(i, j) == 1)
            {
                int newY = tetromino.GetY() + i;
                int newX = tetromino.GetX() + j;
                if (newY < height_ && newX < width_)
                {
                    _blockInfo[newY][newX].occupied = true;
                    _blockInfo[newY][newX].colour = tetromino.getColour();  // Update color
                }
            }
        }
    }
}

bool Board::MoveTetromino(Tetromino& tetromino, int direction)
{
  int Down = 0;
  int Left = 0;
  int Right = 0;
  switch (direction)
  {
    case 1: // Move Tetromino down (1)
      Down = 1;
      break;
    case 2: // Move Tetromino left (2)
      Left = 1;
      break;
    case 3: // Move Tetromino right (3)
      Right = 1;
      break;
  }
  //std::cout << Down << "\n" << Left << "\n" << Right << "\n";

  // Check for collision with occupied positions below
  if (IsCollision(tetromino, Down, Left, Right))
  {
    return true; // Collision detected, movement blocked
  }

  // Clear current Tetromino positions from occupied_
  ClearTetromino(tetromino);

  // Update Tetromino's position
  tetromino.SetPosition(tetromino.GetX() + Right - Left, tetromino.GetY() - Down);

  // Update Tetromino's position on occupied_
  UpdateTetrominoOnBoard(tetromino);
  return false;
}

bool Board::RotateTetromino(Tetromino& tetromino)
{
    // First, simulate rotation to check for potential collisions
    Tetromino tempTetromino = tetromino;  // Make a copy to simulate rotation
    tempTetromino.Rotate();  // Perform rotation on the copy

    // Check if the new position would cause a collision
    if (IsCollision(tempTetromino, 0, 0, 0))
    {
        return false; // If collision, rotation is not performed
    }

    // Clear current Tetromino positions from board
    ClearTetromino(tetromino);

    // Perform actual rotation on original Tetromino
    tetromino.Rotate();

    // Update Tetromino's position on the board
    UpdateTetrominoOnBoard(tetromino);

    return true; // Rotation successful
}

void Board::ClearFullRows()
{
    for (int row = height_ - 1; row >= 0; --row)
    { // Start from the topmost row
        bool rowFull = true;
        for (int col = 0; col < width_-1; ++col)
        {
            if (!_blockInfo[row][col].occupied)
            {
                rowFull = false;
                break;
            }
        }

        if (rowFull) {
            // Move all rows above this one down
            for (int moveRow = row; moveRow < height_ - 1; ++moveRow)
            {
                _blockInfo[moveRow] = _blockInfo[moveRow + 1];
            }

            // Clear the topmost row
            _blockInfo[height_ - 1].assign(width_, {false, ngl::Vec4()}); // Reset the bottommost row

            // Decrease the index to check the same row index again as it now contains what was previously above it
            row++;
            _score++;
        }
    }
}

ngl::Vec4 Board::GetBlock(int row, int col) const
{
    if (row >= 0 && row < height_ && col >= 0 && col < width_)
    {
        return _blockInfo[row][col].occupied ? _blockInfo[row][col].colour : ngl::Vec4(0,0,0,0);
    }
    return ngl::Vec4(0, 0, 0, 0); // Return default color for out-of-bounds
}
