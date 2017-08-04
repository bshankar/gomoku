#include "board.hpp"
#include <iostream>


void Board::print() {
  for (int r = 0; r < 19; ++r) {
    for (int c = 0; c < 19; ++c) {
      if (rows[0][r][c])
        std::cout << "o ";
      else if (rows[1][r][c])
        std::cout << "x ";
      else
        std::cout << ". ";
    }
    std::cout << "\n";
  }
}


bool Board::place(int row, int col, int value) {
  if (rows[0][row][col] || rows[1][row][col])
    return false;

  rows[value][row][col] = true;
  cols[value][col][row] = true;
  diag[value][row - col + 18][row] = true;
  antiDiag[value][row + col][row] = true;
  return true;
}


int Board::winner() {
  for (int i = 0; i < 2; ++i) {
    for (int r = 0; r < 19; ++r)
      if (isWinning(rows[i][r]) || isWinning(cols[i][r]))
        return i;

    for (int d = 0; d < 37; ++d)
      if (isWinning(diag[i][d]) || isWinning(antiDiag[i][d]))
        return i;
  }
  return -1;
}


bool Board::isWinning(std::bitset<19> house) {
  std::bitset<19> bitmask(0b11111);

  for (int i = 0; i < 15; ++i) {
    if ((bitmask & house) == bitmask)
      return true;
    bitmask <<= 1;
  }
  return false;
}


bool Board::isFull() {
  for (int r = 0; r < 19; ++r)
    if (rows[0][r].count() == 19 || rows[1][r].count() == 19)
      return true;
  return false;
}
