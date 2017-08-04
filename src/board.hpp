#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <bitset>

class Board {
public:
  bool place(int row, int col, int val);
  void print();
  int winner(); 
  bool isFull();
  
private:
  std::bitset<19> rows[2][19], cols[2][19],
    diag[2][37], antiDiag[2][37];

  bool isWinning(std::bitset<19> house);
};

#endif
