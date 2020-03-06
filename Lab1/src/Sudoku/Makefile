CXXFLAGS+=-O2 -ggdb -DDEBUG
CXXFLAGS+=-Wall -Wextra

all: sudoku

sudoku: main.cc neighbor.cc sudoku_basic.cc sudoku_min_arity.cc sudoku_min_arity_cache.cc sudoku_dancing_links.cc
	g++ -O2 -o $@ $^

run1: 
	./sudoku test1

run1000:
	./sudoku test1000

clean:
	-rm -f sudoku