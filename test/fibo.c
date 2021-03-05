int fibo(int index)
{
    if (index == 0)
        return 0;
    if (index == 1)
        return 1;
    return fibo(index - 1) + fibo(index - 2);
}

int main()
{
    if (fibo(0) != 0)
        return 1;
    if (fibo(1) != 1)
        return 1;
    if (fibo(2) != 1)
        return 1;
    if (fibo(3) != 2)
        return 1;
    if (fibo(4) != 3)
        return 1;
    if (fibo(5) != 5)
        return 1;
    if (fibo(6) != 8)
        return 1;
    if (fibo(7) != 13)
        return 1;
    if (fibo(8) != 21)
        return 1;
    if (fibo(9) != 34)
        return 1;
    if (fibo(10) != 55)
        return 1;
    if (fibo(11) != 89)
        return 1;
    
    return 0;
}
