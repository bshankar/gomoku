#ifndef BOARD_HPP
#define BOARD_HPP

typedef unsigned int board_t;

class Board {
public:
  bool place(int row, int col, int val);
  void print();
  int winner(); 
  bool isFull();
  
  board_t houses[2][112] = {};
  bool isWinning(board_t house);
};

#endif
