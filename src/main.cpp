#include "search.hpp"
#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::string;

void printHelp() {
  cout << "Gomoku v0.1\n";
  cout << "Usage:\n./gomoku -h (To play human)\n";
  cout << "./gomoku -c (To play the computer)\n";
}

void playHuman() {
  Board board;
  int turn = 0, row, col;
  
  while (board.winner() == -1) {
    board.print();
    cout << "Player " << turn << "s turn.\n";
    cout << "Put stone at: ";
    cin >> row >> col;
    cout << "\n";
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

void playComputer(int depth) {
  cout << "Allow me to play first? (y or n): ";
  string res;
  cin >> res;

  Board board;
  int row, col, turn = 0;
  Search search;
  int computerTurn, playerTurn;
  
  if (res == "y" or res == "yes")
    computerTurn = 0;
  else
    computerTurn = 1;

  playerTurn = computerTurn ^ 1;
  
  while (board.winner() == -1) {
    if (computerTurn == turn) {
      cout << search.negamax(board, depth, -1e100, 1e100, turn) << "\n";
      board = search.pv[depth];
      board.print();
      turn ^= 1;
    } else {
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
  }
  board.print();
  cout << "Player " << board.winner() << " won!\n";  
}

void test(int depth, int turn) {
  Board board;
  Search search;
  Evaluation eval;
  board.place(0, 9, 0);
  board.print();
  cout << eval.compute(board, turn) << "\n";
  cout << "Score: " << search.negamax(board, depth, -1e100, 1e100, turn) << "\n";
  search.pv[depth].print();
}

int main(int argc, char* argv[]) {
  // if (argc == 2) {
    // if (string(argv[1]) == "-h")
      // playHuman();
    // else if (string(argv[1]) == "-c")
      // playComputer(4);
    // else
      // printHelp();
  // } else
    // printHelp();
  test(3, 1);
}
