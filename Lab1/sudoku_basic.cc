#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <pthread.h>
#include <thread>
#include <string.h>
#include <iostream>
#include <map>
#include "sudoku.h"
using namespace std;

pthread_mutex_t queue_mutex =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t output_mutex =PTHREAD_MUTEX_INITIALIZER;
__thread int board[N];   //使用__thread变量使每个线程有一份独立的board实体
//std::queue<std::vector<int>> puzzleSet;   
std::map<int,std::vector<int>> puzzleSet;
map<int,std::vector<int>>::iterator it;

int (*chess)[COL] = (int (*)[COL])board;

void solveSudoku()
{     
  bool (*solve)(int) = solve_sudoku_dancing_links; 
  pthread_mutex_lock(&queue_mutex);
  if(it!=puzzleSet.end()) 
  {
    std::vector<int> tmp=it->second;
    std::copy(tmp.begin(),tmp.end(),board);
    //puzzleSet.pop();
    it++;
  }
  else
  {
    return ;
  }
  pthread_mutex_unlock(&queue_mutex);
  if (solve(0))
  {
    std::vector<int> tmp(board,board+81);
    puzzleSet[it->first]=tmp;
    //output();
    //if (!solved())
      //assert(0);
  }
  return ;
}

void output()
{
  pthread_mutex_lock(&output_mutex);
  it=puzzleSet.begin();
  while(it!=puzzleSet.end())
  {
    cout<<it->first<<endl;
    vector<int> tmp=it->second;
    for(int i=0;i<81;i++) cout<<tmp[i];
    cout<<endl;
    it++;
  }
  pthread_mutex_unlock(&output_mutex);
}

void input(const char in[N],int num)
{
	std::vector<int> tmp;
	for (int cell = 0; cell < N; ++cell)
	{
		tmp.push_back(in[cell] - '0');
		assert(0 <= tmp[cell] && tmp[cell] <= NUM);
	}
	puzzleSet.insert(pair<int,vector<int>>(num,tmp));
	//std::cout<<puzzleSet.size()<<std::endl;
}
