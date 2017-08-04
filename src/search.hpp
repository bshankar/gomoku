#ifndef SEARCH_H
#define SEARCH_H

#include "evaluation.hpp"

class Search {
public:
  Search(Board& board);
  void search(int turn);
};

#endif
