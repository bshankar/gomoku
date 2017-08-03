#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>

class Board {
  /*
    3 states. 0, 1 and 2.
    0 for empty, 
    1 -> player 1 
    2 -> player 2

    So we need 2 bits for each cell. 
    Support a maximum of 32x32 board by default.
   */

  Board(int rows, int colns, int k);
  void place(int row, int col, int value);
  int winner();  // determine the winner
  
private:
  int rows, colns, k;
  std::vector<int> eachRow[2], eachCol[2],
    eachDiag[2], eachAntiDiag[2];
  std::vector<std::vector<int> > board;
  
  void initVectors();
};

#endif
