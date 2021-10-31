// Copyright (c) 2018 Rui Ueyama
// modified partially from https://github.com/rui314/9cc/blob/master/examples/nqueen.c

#include <stdio.h>

int board[10][10];

int print_board()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
            if (board[i][j])
                printf("Q ");
            else
                printf(". ");
        printf("\n");
    }
    printf("\n\n");
}

int conflict(int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        if (board[i][col])
            return 1;
        int j = row - i;
        if (0 < col - j + 1 && board[i][col - j])
            return 1;
        if (col + j < 10 && board[i][col + j])
            return 1;
    }
    return 0;
}

int solve(int row)
{
    if (row > 9)
    {
        print_board();
        return 0;
    }
    for (int i = 0; i < 10; i++)
    {
        if (conflict(row, i))
        {
        }
        else
        {
            board[row][i] = 1;
            solve(row + 1);
            board[row][i] = 0;
        }
    }
}

int main()
{
    solve(0);
    return 0;
}
