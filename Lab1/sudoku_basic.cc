#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <pthread.h>
#include <string.h>
#include <iostream>
#include <thread>

#include "sudoku.h"

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;
__thread int board[N]; //使用__thread变量使每个线程有一份独立的board实体
std::queue<std::vector<int>> puzzleSet;
std::queue<std::vector<int>> puzzleSetCopyForSingleThread;
int (*chess)[COL] = (int (*)[COL])board;

void solveSudoku()
{
	// std::cout << "The solveSudoku thread ID is " << std::this_thread::get_id() << "\n"; //debug
	bool (*solve)(int) = solve_sudoku_dancing_links;
	pthread_mutex_lock(&queue_mutex);
	if (!puzzleSet.empty())
	{
		std::vector<int> tmp = puzzleSet.front();
		std::copy(tmp.begin(), tmp.end(), board);
		puzzleSet.pop();
	}
	else
		return;
	if (solve(0))
	{
		output();  //暂时不输出
		//if (!solved())
		//assert(0);
	}
	pthread_mutex_unlock(&queue_mutex);
}

void my_solveSudoku()
{
	// std::cout << "The solveSudoku thread ID is " << std::this_thread::get_id() << "\n"; //debug
	bool (*solve)(int) = solve_sudoku_dancing_links;
	pthread_mutex_lock(&queue_mutex);
	if (!puzzleSet.empty())
	{
		std::vector<int> tmp = puzzleSet.front();
		std::copy(tmp.begin(), tmp.end(), board);
		puzzleSet.pop();
	}
	else
		return;
	if (solve(0))
	{
		output();  //暂时不输出
		//if (!solved())
		//assert(0);
	}
	pthread_mutex_unlock(&queue_mutex);
}

void output()
{
	pthread_mutex_lock(&output_mutex);
	std::cout << std::this_thread::get_id() << " :";	//debug
	for (int i = 0; i < N; i++)
	{
		printf("%d", board[i]);
	}
	printf("\n");
	pthread_mutex_unlock(&output_mutex);
}

void input(const char in[N])
{
	std::vector<int> tmp;
	for (int cell = 0; cell < N; ++cell)
	{
		tmp.push_back(in[cell] - '0');
		assert(0 <= tmp[cell] && tmp[cell] <= NUM);
	}
	puzzleSet.push(tmp);
	//std::cout<<puzzleSet.size()<<std::endl;
}
