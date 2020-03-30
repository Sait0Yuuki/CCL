#include <queue>
#include <atomic>
#include <map>

#ifndef SUDOKU_H
#define SUDOKU_H

#define THREADNUM 4

const bool DEBUG_MODE = false;
enum { ROW=9, COL=9, N = 81, NEIGHBOR = 20 };
const int NUM = 9;

extern __thread int neighbors[N][NEIGHBOR];
extern __thread int board[N]; 
extern int spaces[N];
extern int nspaces;
extern int (*chess)[COL];
extern std::map<int,std::vector<int>> puzzleSet;
extern std::map<int,std::vector<int>>::iterator it;

void solveSudoku();

void init_neighbors();
void input(const char in[N],int i);
void init_cache();
void output();

bool available(int guess, int cell);

bool solve_sudoku_basic(int which_space);
bool solve_sudoku_min_arity(int which_space);
bool solve_sudoku_min_arity_cache(int which_space);
bool solve_sudoku_dancing_links(int unused);
bool solved();
#endif
