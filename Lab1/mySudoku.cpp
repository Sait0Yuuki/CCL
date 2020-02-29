#include <iostream>
#include <vector>
#include <thread>
using namespace std;

int board[9][9] = {
    {0, 0, 5, 3, 0, 0, 0, 0, 0},
    {8, 0, 0, 0, 0, 0, 0, 2, 0},
    {0, 7, 0, 0, 1, 0, 5, 0, 0},
    {4, 0, 0, 0, 0, 5, 3, 0, 0},
    {0, 1, 0, 0, 7, 0, 0, 0, 6},
    {0, 0, 3, 2, 0, 0, 0, 8, 0},
    {0, 6, 0, 5, 0, 0, 0, 0, 9},
    {0, 0, 4, 0, 0, 0, 0, 3, 0},
    {0, 0, 0, 0, 0, 9, 7, 0, 0}};

bool check(int i, int j, char val)
{
    int row = i - i%3, column = j - j%3;
    for(int x=0; x<9; x++) if(board[x][j] == val) return false;
    for(int y=0; y<9; y++) if(board[i][y] == val) return false;
    for(int x=0; x<3; x++)
    for(int y=0; y<3; y++)
        if(board[row+x][column+y] == val) return false;
    return true;
}

void solveSudoku(int i, int j)
{
    if (i == 8 && j == 9)
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                cout << board[i][j] << ' ';
            }
            cout << endl;
        }
        cout << endl;
        return;
    }
    if (j > 8)
    {
        i++;
        j = 0;
    }
    if (board[i][j] == 0)
    {
        for (int num = 1; num <= 9; num++)
        {
            if (check(i, j, num))
            {
                board[i][j] = num;
                thread t1(solveSudoku,i,j+1);
                t1.join();
            }
            board[i][j] = 0;
        }
    }
    else
    {
        thread t2(solveSudoku,i,j+1);
        t2.join();
    }
}

int main()
{
    solveSudoku(0, 0);
}