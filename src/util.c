#include "hooligan.h"

#include "util/error.c"
#include "util/string.c"
#include "util/type.c"



int f2bin(float x)
{
    if (x == 0.0)
    {
        return 0;
    }
    int bin[32]; // IEEE754の表現
    int mid[24]; // 中間表現では2進数だがIEEE754ではない

    for (int i = 0; i < 32; i++)
    {
        bin[i] = 0;
    }

    for (int i = 0; i < 24; i++)
    {
        mid[i] = 0;
    }

    // 符号部の処理
    if (x < 0.0)
    {
        bin[0] = 1;
        x *= -1.0;
    }

    // xを2進数に変換
    // 23桁まで計算

    // 整数部の計算
    // 割れる最大の2^nを見つける
    int n;
    if (x <= 1.0)
    {
        n = 0;
    }
    else
    {
        float power = 1.0;
        n = 1;
        while (x >= 2.0 * power)
        {
            power *= 2.0;
            n += 1;
        }

        for (int i = 0; i < n; i++)
        {
            if (x >= power)
            {
                mid[i] = 1;
                x = x - power;
            }
            else
            {
                mid[i] = 0;
            }
            power /= 2.0;
        }
    }
    // ここでは x<= 1.0になっている

    int i = n;
    if (n == 0)
    {
        while (x < 1.0 && x != 0.0)
        {
            x *= 2.0;
            n -= 1;
        }
    }
    else
    {
        x *= 2.0;
        n = n - 1;
    }

    for (; i < 24; i++)
    {
        if (x == 0.0)
        {
            break;
        }

        if (x >= 1.0)
        {
            mid[i] = 1;
            x = x - 1.0;
        }
        x *= 2.0;
    }

    // IEEE754への変換

    // 指数部
    int keta = 127 + n;
    int power = 128;
    for (int j = 1; j < 9; j++)
    {
        if (keta >= power)
        {
            bin[j] = 1;
            keta -= power;
        }
        power = power / 2;
    }

    // 仮数部分
    for (int j = 9; j < 32; j++)
    {
        // bin[9]は必ず0
        bin[j] = mid[j - 8];
    }

    // 10進数への変換
    int res = 0;
    power = 1;
    for (int j = 0; j < 32; j++)
    {
        res += bin[31 - j] * power;
        power *= 2;
    }
    //printf("%d\n", res);
    return res;
}
