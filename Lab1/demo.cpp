#include<iostream>
#include<pthread.h>
#include "ThreadPool.h"
using namespace std;

pthread_mutex_t counter_mutex =PTHREAD_MUTEX_INITIALIZER;
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
    ThreadPool pool(4);
    for(int i=0;i<100;i++)
    {
        pool.enqueue(fun);
    }
}
