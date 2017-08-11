#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>

typedef uint64_t Hash;
typedef uint32_t House;
typedef uint16_t Move;
typedef uint16_t Key;
typedef int32_t Eval;
typedef int8_t HasWon;


class Board {
public:
  Board();
  bool place(Move move, bool player);
  bool remove(Move move, bool player);
  void print();
  int winner(); 
  bool isFull();
  bool isEmpty();
  bool isWinning(House house);

  House houses[2][112] = {};
  HasWon hasWon = -1;

  // evaluation related stuff
  Eval eval = 0;
  Eval partialEvals[112] = {};
  Eval compareHouses(House h1, House h2);
  void updatePartials(Move move, bool player);
  void updateEval(Move move, bool player);

  // zobrist hashing
  Hash hash;
  Hash rtable[2][361]; // random number table for hashing
  void updateHash(Move move, int player);
};

#endif
