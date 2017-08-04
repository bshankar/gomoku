#include "board.hpp"
#include <iostream>

using std::cout;
using std::cin;

int main(int argc, char* argv[]) {
  Board board;
  int turn = 0, row, col;
  
  while (board.winner() == -1) {
    board.print();
    cout << "Player " << turn << "s turn.\n";
    cout << "Put stone at: ";
    cin >> row >> col;
    cout << "\n";

    if (row >= 0 && row < 19 &&
        col >= 0 && col < 19 &&
        board.place(row, col, turn))
      turn ^= 1;
    else 
      cout << "Invalid cell. Try again.\n";
  }

  board.print();
  cout << "Player " << board.winner() << " won!\n";
}
