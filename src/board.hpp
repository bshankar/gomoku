#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include "evaluate.hpp"

typedef uint64_t Hash;
typedef uint32_t House;
typedef uint16_t Move;
typedef uint16_t Key;
typedef int8_t HasWon;


class Board {
public:
  Board();
  bool place(bool player, Move move);
  bool remove(bool player, Move move);
  bool isWinning(House house);
  bool isFull();
  bool isEmpty();
  bool isFilledAt(Move h, Move i);
  int winner(); 
  void print();
  House getHouse(bool player, Move move);
  Hash getHash();
  Evaluate& getEvaluate();
  
  struct Moves {
    Move moveArray[361];
    int end = 0;
  };

  Moves getMovesMade();

private:
  Evaluate eval;
  House houses[2][112] = {};
  HasWon hasWon = -1;
  Hash hash;
  Hash rtable[2][361]; 
  Moves movesMade;
  void updateHash(bool player, Move move);
  void updatePartials(bool player, Move move);
  void updateEval(bool player, Move move);
};

#endif
