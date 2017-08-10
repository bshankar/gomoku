#include "board.hpp"
#include <iostream>
#include <random>
#include <limits>

Board::Board() {
  // initialize rtable
  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t>
    distribution(std::numeric_limits<uint64_t>::min(),
                 std::numeric_limits<uint64_t>::max());

  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 361; ++j)
        rtable[i][j] = distribution(generator);
}


void Board::print() {
  std::cout << " ";
  for (int i = 0; i < 19; ++i)
    std::cout << " " << i % 10;
  std::cout << "\n";
  
  for (int r = 0; r < 19; ++r) {
    std::cout << r % 10 << " ";
    for (int c = 0; c < 19; ++c) {
      if (houses[0][r] & (1 << c))
        std::cout << "O ";
      else if (houses[1][r] & (1 << c))
        std::cout << "X ";
      else
        std::cout << "· ";
    }
    std::cout << "\n";
  }
}


bool Board::place(Move move, bool player) {
  auto row = move/19;
  auto col = move % 19;
  
  if (houses[0][row] & (1 << col) ||
      houses[1][row] & (1 << col))
    return false;

  houses[player][row] |= (1 << col);
  houses[player][19 + col] |= (1 << row); 
  houses[player][38 + row - col + 18] |= (1 << row);
  houses[player][75 + row + col] |= (1 << row);

  if (isWinning(houses[player][row]) || isWinning(houses[player][19 + col]) ||
      isWinning(houses[player][38 + row - col + 18]) ||
      isWinning(houses[player][75 + row + col]))
    hasWon = player;

  updateEval(move, player);
  updateHash(move, player);
  return true;
}


bool Board::remove(Move move, bool player) {
  auto row = move/19;
  auto col = move % 19;
  
  if (houses[player][row] & (1 << col)) {
    houses[player][row] ^= (1 << col);
    houses[player][19 + col] ^= (1 << row);
    houses[player][38 + row - col + 18] ^= (1 << row);
    houses[player][75 + row + col] ^= (1 << row);

    if (hasWon == 1)
      eval += 500000;
    else if (!hasWon)
      eval -= 500000;
    
    // assume we don't search after somebody won
    hasWon = -1;
    updateEval(move, player);
    updateHash(move, player);
    return true;
  }
  return false;
}


int Board::winner() {
  return hasWon;
}


bool Board::isWinning(House house) {
  House bitmask = 0b11111;

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


void Board::updatePartials(Move move, bool player) {
  auto row = move/19;
  auto col = move % 19;
  
  auto housesToChange = {row, 19 + col, 38 + row - col + 18, 75 + row + col};
  for (auto h: housesToChange) {
    auto prev = partialEvals[h];
    auto next = compareHouses(houses[0][h], houses[1][h]);
    eval += next - prev;
    partialEvals[h] = next;
  }
}


void Board::updateEval(Move move, bool player) {
  if (!hasWon)
    eval += 500000;
  else if (hasWon == 1)
    eval -= 500000;
  updatePartials(move, player);
}


Eval Board::compareHouses(House h1, House h2) {
  House bitmask = 0b11111;
  double eval = 0;

  double patternScores[] = {0, 1, 21, 337, 4045};
  
  while (h1 | h2) {
    auto res = h1 & bitmask,
      resOpp = h2 & bitmask;

    if (res != 0 && resOpp == 0) {
      eval += patternScores[__builtin_popcount(res)];
    }
    
    else if (res == 0 && resOpp != 0) {
      eval -= patternScores[__builtin_popcount(resOpp)];
    }

    h1 >>= 1;
    h2 >>= 1;
  }
  return eval;
}


void Board::updateHash(Move move, int player) {
  hash ^= rtable[player][move];
}
