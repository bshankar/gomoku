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

    if (row >= 0 && row < 19 &&
        col >= 0 && col < 19 &&
        board.place(row*19 + col, turn)) {
      cout << board.eval << "\n";
      turn ^= 1;
    }
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
  Search search(board);
  int computerTurn, playerTurn;
  
  if (res == "y" or res == "yes")
    computerTurn = 0;
  else
    computerTurn = 1;
  
  playerTurn = computerTurn ^ 1;
  
  while (board.winner() == -1) {
    if (computerTurn == turn) {
      auto move = search.calcBestMove(depth, turn); 
      board.place(move, turn);
      board.print();
      turn ^= 1;
    } else {
      cout << "Player " << turn << "s turn.\n";
      cout << "Put stone at: ";
      cin >> row >> col;
      cout << "\n";
                                
      if (row >= 0 && row < 19 &&  // 
          col >= 0 && col < 19 &&
          board.place(row*19 + col, turn)) 
        turn ^= 1;
      else 
        cout << "Invalid cell. Try again.\n";
    }
  }
  board.print();
  cout << "Player " << board.winner() << " won!\n";  
}


void playItself(int depth) {
  Board board;
  Search search(board);
  int turn = 0;
  while (board.winner() == -1 && !board.isFull()) {
    auto move = search.calcBestMove(depth, turn); 
      board.place(move, turn);
      board.print();
      turn ^= 1;
  }
  if (board.winner() != -1) {
    cout << "Player " << board.winner() << " won\n";
  } else
    cout << "Game drawn!\n";
}

  
void test(int depth, int turn) {
  Board board;
  for (int i = 9; i >= 5; --i) {
    board.place(i*19 + i, 1);
    board.print();
    cout << board.eval << " " << board.hash << "\n";
  }

  for (int i = 5; i < 10; ++i) {
    board.remove(i*19 + i, 1);
    board.print();
    cout << board.eval << " " << board.hash << "\n";
  }
  board.place(9*19 + 9, 0);
  board.print();
  Search search(board);
  search.calcBestMove(depth, turn);
}


int main(int argc, char* argv[]) {
  if (argc == 2) {
    if (string(argv[1]) == "-h")
      playHuman();
    else
      printHelp();
  } else if (argc == 3) {
    if (string(argv[1]) == "-c") {
      playComputer(atoi(argv[2]));
    }
    else if (string(argv[1]) == "-t")
      test(atoi(argv[2]), 1);
    else if (string(argv[1]) == "-i")
      playItself(atoi(argv[2]));
    else
      printHelp();
  } else
    printHelp();
}
