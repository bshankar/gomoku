#include "board.hpp"
#include "evaluate.hpp"
#include <iostream>
#include <random>
#include <limits>

Board::Board() {
  // initialize rtable for zobrist hashing
  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t>
    distribution(std::numeric_limits<uint64_t>::min(),
                 std::numeric_limits<uint64_t>::max());

  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 361; ++j)
        rtable[i][j] = distribution(generator);
}


void Board::print() {
  // print the board
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


bool Board::place(bool player, Move move) {
  auto row = move/19;
  auto col = move % 19;
  
  if (houses[0][row] & (1 << col) ||
      houses[1][row] & (1 << col))
    // this cell is already occupied so exit 
    return false;

  // fill the appropriate houses
  houses[player][row] |= (1 << col);
  houses[player][19 + col] |= (1 << row); 
  houses[player][38 + row - col + 18] |= (1 << row);
  houses[player][75 + row + col] |= (1 << row);

  // check if player is winning
  if (isWinning(houses[player][row]) || isWinning(houses[player][19 + col]) ||
      isWinning(houses[player][38 + row - col + 18]) ||
      isWinning(houses[player][75 + row + col]))
    hasWon = player;

  updateEval(player, move);
  updateHash(player, move);
  movesMade.moveArray[movesMade.end] = move;
  ++movesMade.end;
  return true;
}


bool Board::remove(bool player, Move move) {
  auto row = move/19;
  auto col = move % 19;
  
  if (houses[player][row] & (1 << col)) {
    // remove values from appropriate houses
    houses[player][row] ^= (1 << col);
    houses[player][19 + col] ^= (1 << row);
    houses[player][38 + row - col + 18] ^= (1 << row);
    houses[player][75 + row + col] ^= (1 << row);

    // if previously won, update the eval
    if (!hasWon)
      eval.addToEval(-500000);
    else if (hasWon == 1)
      eval.addToEval(500000);
    
    // assume we don't search after somebody won
    hasWon = -1;
    updateEval(player, move);
    updateHash(player, move);
    --movesMade.end;
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


bool Board::isEmpty() {
  for (int r = 0; r < 19; ++r)
    if (houses[0][r])
      return false;
  return true;
}


void Board::updateHash(bool player, Move move) {
  hash ^= rtable[player][move];
}


void Board::updatePartials(bool player, Move move) {
  auto row = move/19;
  auto col = move % 19;
  
  auto housesToChange = {row, 19 + col, 38 + row - col + 18, 75 + row + col};
  for (auto h: housesToChange) {
    auto prev = eval.getPartialEval(h);
    auto next = eval.compareHouses(houses[0][h], houses[1][h]);
    eval.addToEval(next - prev);
    eval.setPartialEval(h, next);
  }
}


void Board::updateEval(bool player, Move move) {
  if (!hasWon)
    eval.addToEval(500000);
  else if (hasWon == 1)
    eval.addToEval(-500000);
  updatePartials(player, move);
}


House Board::getHouse(bool player, Move move) {
  return houses[player][move];
}


Hash Board::getHash() {
  return hash;
}


Evaluate& Board::getEvaluate() {
  return eval;
}


Board::Moves Board::getMovesMade() {
  return movesMade;
}


bool Board::isFilledAt(Move h, Move i) {
  if (houses[0][h] & (1 << i) || houses[1][h] & (1 << i))
    return true;
  return false;
}
