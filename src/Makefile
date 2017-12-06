export EMCC_DEBUG=1

gomoku.js:
	em++ --bind board.cpp search.cpp tt.cpp evaluate.cpp -std=c++11 -O3 -s WASM=1 -o ../bin/gomoku.js
clean:
	rm *.js *.wasm
