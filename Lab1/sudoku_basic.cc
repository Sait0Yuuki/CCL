#include <assert.h>
#include <stdio.h>

#include <algorithm>

#include "sudoku.h"

int board[N];
int spaces[N];
int nspaces;
int (*chess)[COL] = (int (*)[COL])board;

static void find_spaces()
{
  nspaces = 0;
  for (int cell = 0; cell < N; ++cell) {
    if (board[cell] == 0)
      spaces[nspaces++] = cell;
  }
}

void output()
{
  for(int i=0;i<N;i++) printf("%d",board[i]);
  printf("\n");
}

void input(const char in[N])
{
  for (int cell = 0; cell < N; ++cell) {
    board[cell] = in[cell] - '0';
    assert(0 <= board[cell] && board[cell] <= NUM);
  }
  find_spaces();
}

