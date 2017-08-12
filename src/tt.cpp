#include "tt.hpp"
typedef TTable::TTEntry TTEntry;


TTable::TTable() {
  hashTable.resize(1 << 24);
}


TTEntry TTable::probe(Hash hash) {
  return hashTable[hash & 0xffffff];
}


void TTable::store(TTEntry entry, Hash hash) {
  hashTable[hash & 0xffffff] = entry;
}
