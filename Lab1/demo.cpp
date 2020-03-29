#include<iostream>
#include<pthread.h>
#include "ThreadPool.h"
using namespace std;

pthread_mutex_t counter_mutex =PTHREAD_MUTEX_INITIALIZER;
//__thread int sum=0;  //___thread变量每一个线程有一份独立实体
int sum=0;

void fun()
{
    pthread_mutex_lock(&counter_mutex);
    sum++;
    cout<<"sum: "<<sum<<endl;
    cout<<"Thread ID: "<<this_thread::get_id()<<endl;
    pthread_mutex_unlock(&counter_mutex);
}

int main()
{
    std::queue<int[10]> puzzleSet;
    int tmp[10]={0,1,2,3,4,5,6,7,8,9};
    puzzleSet.push(tmp);
    ThreadPool pool(4);
    for(int i=0;i<100;i++)
    {
        pool.enqueue(fun);
    }
    return 0;
}
