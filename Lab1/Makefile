CXXFLAGS+=-O2 -ggdb -DDEBUG
CXXFLAGS+=-Wall -Wextra

all: sudoku
.PHONY: clean

sudoku: main.cc neighbor.cc sudoku_basic.cc  sudoku_dancing_links.cc
	g++ -O2 -o sudoku main.cc neighbor.cc sudoku_basic.cc  sudoku_dancing_links.cc

demo: demo.cpp ThreadPool.h
	g++ -pthread -o demo demo.cpp

clean:
	rm demo sudoku