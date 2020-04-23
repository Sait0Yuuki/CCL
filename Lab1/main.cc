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
	ThreadPool inputpool(THREADNUM);
	init_neighbors();
	//FILE *fp;
	//fp[0] = fopen(argv[1], "r");
	char puzzle[128];

	int total_solved = 0;
	int total = 0;
    char *FileName=(char*)malloc(256*sizeof(char));
	FILE *fp;
	while(fgets(FileName, 256, stdin)) {
        if(FileName[0]=='\n') {
			printf("stop reading the file,please wait\n");
			break;
		}
        if(FileName[strlen(FileName)-1]=='\n')
			FileName[strlen(FileName)-1]='\0';

		fp = fopen(FileName, "r");

		if(fp==NULL) {
			printf("%s dose not have data\n",FileName);
			continue;
		}

			// std::string FileName_str = "";
			// std::cin>>FileName_str;
			// FileName_str.erase(0,2);
			// fp = fopen(FileName_str.c_str(), "rw");
			while (fgets(puzzle, sizeof puzzle, fp) != NULL)
			{
				if (strlen(puzzle) >= N)
				{
				//	inputpool.enqueue(input(puzzle,total++));
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
		usleep(100); //sleep to avoid dead loop
	}
	duration<double> diff = system_clock::now() - start;

	output();
	
	std::cout <<"Thread Num: "<<THREADNUM<< " Runtime: " << diff.count() << "s.\n";
	return 0;
}
