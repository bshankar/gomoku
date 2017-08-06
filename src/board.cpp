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
  houses[value][75 + row + col] |= (1 << row);

  if (isWinning(houses[value][row]) || isWinning(houses[value][19 + col]) ||
      isWinning(houses[value][38 + row - col + 18]) ||
      isWinning(houses[value][75 + row + col]))
    hasWon = value;

  updateEval(row, col, value);
  return true;
}


bool Board::remove(int row, int col, int value) {
  if (houses[value][row] & (1 << col)) {
    houses[value][row] ^= (1 << col);
    houses[value][19 + col] ^= (1 << row);
    houses[value][38 + row - col + 18] ^= (1 << row);
    houses[value][75 + row + col] ^= (1 << row);

    // assume we don't search after somebody won
    hasWon = -1;
    updateEval(row, col, value);
    return true;
  }
  return false;
}


bool Board::place(Move move) {
  return place(move.row, move.col, move.value);
}


bool Board::remove(Move move) {
  return remove(move.row, move.col, move.value);
}


int Board::winner() {
  return hasWon;
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
    if ((houses[0][r] | houses[1][r]) != 0x7ffff)
      return false;
  return true;
}


void Board::updateCenter(int row, int col, int value) {
  if (houses[value][row] & (1 << col)) {
    // value was inserted here
    if (value)
      eval -= 18 - abs(row - 19/2) - abs(col - 19/2);
    else
      eval += 18 - abs(row - 19/2) - abs(col - 19/2);
  } else {
    // value was removed
    if (value)
      eval += 18 - abs(row - 19/2) - abs(col - 19/2);
    else
      eval -= 18 - abs(row - 19/2) - abs(col - 19/2);
  }
}


void Board::updatePartials(int row, int col, int value) {
  auto housesToChange = {row, 19 + col, 38 + row - col + 18, 75 + row + col};
  for (auto h: housesToChange) {
    auto prev = partialEvals[h];
    auto next = compareHouses(houses[0][h], houses[1][h]);
    eval += next - prev;
    partialEvals[h] = next;
  }
}


void Board::updateEval(int row, int col, int value) {
  if (!hasWon) {
    eval = 1e100;
    return;
  }
  else if (hasWon == 1) {
    eval = -1e100;
    return;
  }
  updateCenter(row, col, value);
  updatePartials(row, col, value);
}


double Board::compareHouses(board_t h1, board_t h2) {
  board_t bitmask = 0b11111;
  double eval = 0;
  
  while (h1 | h2) {
    auto res = h1 & bitmask,
      resOpp = h2 & bitmask;

    if (res != 0 && resOpp == 0)
      eval += 100*(1 << __builtin_popcount(res));
    else if (res == 0 && resOpp != 0)
      eval -= 100*(1 << __builtin_popcount(resOpp));

    h1 >>= 1;
    h2 >>= 1;
  }
  return eval;
}
