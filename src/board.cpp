#include "board.hpp"
#include <iostream>


void Board::print() {
  for (int r = 0; r < 19; ++r) {
    for (int c = 0; c < 19; ++c) {
      if (houses[0][r] & (1 << c))
        std::cout << "o ";
      else if (houses[1][r] & (1 << c))
        std::cout << "x ";
      else
        std::cout << ". ";
    }
    std::cout << "\n";
  }
}


bool Board::place(int row, int col, int value) {
  if (houses[0][row] & (1 << col) ||
      houses[1][row] & (1 << col))
    return false;

  houses[value][row] |= (1 << col);
  houses[value][19 + col] |= (1 << row); 
  houses[value][38 + row - col + 18] |= (1 << row);
  houses[value][row + col] |= (1 << row);
  return true;
}


int Board::winner() {
  for (int i = 0; i < 2; ++i)
    for (int h = 0; h < 112; ++h) 
      if (isWinning(houses[i][h]))
        return i;
  
  return -1;
}


bool Board::isWinning(board_t house) {
  board_t bitmask = 0b11111;

  while (house) {
    if ((house & bitmask) == bitmask)
      return true;
    
    if (house & 1)
      house--;
    house >>= __builtin_ffs(house) - 1;
  }
  
  return false;
}


bool Board::isFull() {
  for (int r = 0; r < 19; ++r)
    if (houses[0][r] != 0x7fff || houses[1][r] != 0x7fff)
      return false;
  return true;
}
