#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>
#include "sudoku.h"
#include "ThreadPool.h"

int main(int argc, char *argv[])
{
	init_neighbors();
	FILE *fp;
	//fp[0] = fopen(argv[1], "r");
	char puzzle[128];

	int total_solved = 0;
	int total = 0;
	//int64_t start = now();
	for (int i = 1; i < argc; i++)
	{
		fp = fopen(argv[i], "rw");
		while (fgets(puzzle, sizeof puzzle, fp) != NULL)
		{
			if (strlen(puzzle) >= N)
			{
				input(puzzle, total++);
			}
		}
	}

	using namespace std::chrono;
	auto start = system_clock::now();
	ThreadPool pool(THREADNUM);
	it = puzzleSet.begin();
	while (it != puzzleSet.end())
	{
		pool.enqueue(solveSudoku);
		usleep(1000); //sleep to avoid dead loop
	}
	duration<double> diff = system_clock::now() - start;

	output();
	
	std::cout <<"Thread Num: "<<THREADNUM<< " Runtime: " << diff.count() << "s.\n";
	return 0;
}
