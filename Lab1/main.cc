#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <thread>
#include <iostream>

#include "sudoku.h"
#include "ThreadPool.h"

using namespace std;

#define OPEN_SINGLE_THEAD 0
#define OPEN_MULT_THEAD 0
#define OPEN_MY_MULT_THEAD 1

// void myMultThreadFunc(void solveSudoku());

int64_t now()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main(int argc, char *argv[])
{
	init_neighbors();
	FILE *fp = fopen(argv[1], "r");
	char puzzle[128];
	int total_solved = 0;
	int total = 0;
	//int64_t start = now();
	while (fgets(puzzle, sizeof puzzle, fp) != NULL)
	{
		if (strlen(puzzle) >= N)
		{
			++total;
			input(puzzle);
			// printf("No%4d: %s", total, puzzle);  //debug
		}
		else
		{
			printf("No: %s\n", puzzle);
		}
	}

	{
		using namespace std::chrono;
		auto start = system_clock::now();
		if(OPEN_SINGLE_THEAD)
		{
			printf("This part is single thread: \n");
			while (!puzzleSet.empty())
			{
				solveSudoku();
			}
		}
		duration<double> diff = system_clock::now() - start;
		cout << "single_thread run: " << diff.count() << "s.\n";
	}

	// myMultThreadFunc(solveSudoku);
	{
		using namespace std::chrono;
		auto start = system_clock::now();
		if(OPEN_MY_MULT_THEAD)
		{
			printf("This part is my thread thread: \n");
			int puzzleSetSize = puzzleSet.size();
			thread thArr[THREADNUM];
			int i=0;
			while(!puzzleSet.empty())
			{
				for(i=0; i<THREADNUM; i++)
				{
					thArr[i] = thread(solveSudoku);
					// cout << "This thread ID is " << this_thread::get_id() << "\n"; //debug
				}
				for(i=0; i<THREADNUM; i++)
				{
					thArr[i].join();
				}
			}
		}
		duration<double> diff = system_clock::now() - start;
		cout << "mult_thread run: " << diff.count() << "s.\n";
	}

	cout << "The main PID is " << this_thread::get_id() << "\n"; //debug

	
	if(OPEN_MULT_THEAD)
	{
		ThreadPool pool(THREADNUM);
		while (!puzzleSet.empty())
		{

			pool.enqueue(solveSudoku);
		}
	}
	
	//exit(1);
	//int64_t end = now();
	//double sec = (end-start)/1000000.0;
	//printf("%f sec %f ms each %d\n", sec, 1000*sec/total, total_solved);
	return 0;
}

// template <class T>
// void measure(T&& func)
// {
// 	auto start = system_clock::now();

// }

// void myMultThreadFunc(void solveSudoku())
// {
// 	if(OPEN_MY_MULT_THEAD)
// 	{
// 		printf("This part is my thread thread: \n");
// 		int puzzleSetSize = puzzleSet.size();
// 		thread thArr[THREADNUM];
// 		int i=0;
// 		while(!puzzleSet.empty())
// 		{
// 			for(i=0; i<THREADNUM; i++)
// 			{
// 				thArr[i] = thread(solveSudoku);
// 				// cout << "This thread ID is " << this_thread::get_id() << "\n"; //debug
// 			}
// 			for(i=0; i<THREADNUM; i++)
// 			{
// 				thArr[i].join();
// 			}
// 		}
// 	}
// }