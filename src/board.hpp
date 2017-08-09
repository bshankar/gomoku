#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
typedef uint32_t board_t;

class Board {
public:
  Board();
  bool place(int row, int col, int val);
  bool remove(int row, int col, int val);
  void print();
  int winner(); 
  bool isFull();
  bool isWinning(board_t house);

  board_t houses[2][112] = {};
  int hasWon = -1;

  struct Move {
    int row = -1;
    int col = -1;
    int value = -1;
  };
  
  bool place(Move move);
  bool remove(Move move);

  // evaluation related stuff
  double eval = 0;
  double partialEvals[112] = {};
  double compareHouses(board_t h1, board_t h2);
  void updateCenter(int row, int col, int value);
  void updatePartials(int row, int col, int value);
  void updateEval(int row, int col, int value);

  // zobrist hashing
  uint64_t hash;
  uint64_t rtable[2][19][19]; // random number table for hashing
  void updateHash(int row, int col, int value);
};

#endif
