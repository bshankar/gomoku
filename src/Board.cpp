#include "board.hpp"

Board::Board(int rows, int colns, int k) {
  this->rows = rows;
  this->colns = colns;
  this->k = k;

  initVectors();
}

void Board::initVectors() {
  for (int i = 0; i < 2; ++i) {
    eachRow[i].resize(colns);
    eachCol[i].resize(rows);
    eachDiag[i].resize(rows + colns - 1);
    eachAntiDiag[i].resize(rows + colns - 1);
  }
  board.resize(colns, std::vector<int>(rows));
}

int Board::winner() {
  for (int i = 0; i < 2; ++i) {

    for (int r = 0; r < rows; ++r)
      if (eachRow[i][r] >= k)
        return i;

    for (int c = 0; c < colns; ++c)
      if (eachCol[i][c] >= k)
        return i;

    for (int d = 0; d < rows + colns - 1; ++d)
      if (eachDiag[i][d] >= k || eachAntiDiag[i][d] >= k)
        return i;
  }
  return 0;
}

void Board::place(int row, int col, int value) {
  board[row][col] = value;
  
  eachRow[value][row]++;
  eachCol[value][col]++;
  eachDiag[value][row - col + colns - 1]++;
  eachAntiDiag[value][row + col]++;
}
