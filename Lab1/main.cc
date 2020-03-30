#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>
#include "sudoku.h"
#include "ThreadPool.h"

int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}


int main(int argc, char* argv[])
{
  init_neighbors();
  FILE* fp;
  //fp[0] = fopen(argv[1], "r");
  char puzzle[128];
  
  int total_solved = 0;
  int total = 0;
  //int64_t start = now();
  for(int i=1;i<argc;i++)
  {
     fp = fopen(argv[i], "rw");
     while (fgets(puzzle, sizeof puzzle, fp) != NULL) {
      if (strlen(puzzle) >= N) {
      input(puzzle,total++);
      }
      else {
        printf("No: %s", puzzle);
      }
  }
  }
  
  ThreadPool pool(THREADNUM);
  it=puzzleSet.begin();
  while(it!=puzzleSet.end()){
    pool.enqueue(solveSudoku);
    usleep(1000); //sleep to avoid dead loop
  }
  output();

  //exit(1);
  //int64_t end = now();
  //double sec = (end-start)/1000000.0;
  //printf("%f sec %f ms each %d\n", sec, 1000*sec/total, total_solved);
  return 0;
}

