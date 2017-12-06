export EMCC_DEBUG=1

gomoku.js:
	emcc --bind board.cpp search.cpp tt.cpp evaluate.cpp -std=c++11 -O3 -s WASM=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 -s ALLOW_MEMORY_GROWTH=1 -o ../../gomoku.js
clean:
	rm *.js *.wasm
