#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
typedef uint32_t board_t;
typedef uint16_t row_t;
typedef int16_t eval_t;

class Board {
public:
  Board();
  bool place(row_t move, bool val);
  bool remove(row_t move, bool val);
  void print();
  int winner(); 
  bool isFull();
  bool isWinning(board_t house);

  board_t houses[2][112] = {};
  int8_t hasWon = -1;

  // evaluation related stuff
  eval_t eval = 0;
  eval_t partialEvals[112] = {};
  eval_t compareHouses(board_t h1, board_t h2);
  void updatePartials(row_t move, bool value);
  void updateEval(row_t move, bool value);

  // zobrist hashing
  uint64_t hash;
  uint64_t rtable[2][361]; // random number table for hashing
  void updateHash(row_t move, int value);
};

#endif
