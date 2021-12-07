#include <stdio.h>

float fx(float x, float v)
{
    return v;
}

float fv(float x, float v)
{
    float m = 1.0;
    float k = 1.0;
    float gamma = 0.0;
    return -(k / m) * x;
}

float aabs(float x)
{
    if (x >= 0.0)
    {
        return x;
    }
    return -1.0 * x;
}

int run_odd()
{
    float dt = 0.001;
    float x;
    float v;
    float x1;
    float v1;
    float x2;
    float v2;
    float x3;
    float v3;
    float x4;
    float v4;
    float ffstep = 0.0;
    float time;
    x = 0.0;
    v = -1.0;
    for (int step = 0; step < 9999; step++)
    {
        time = ffstep * dt;
        // printf("%.4f %.4f %.4f\n", time, x, v);
        x1 = fx(x, v) * dt;
        v1 = fv(x, v) * dt;
        x2 = fx(x + x1 / 2.0, v + v1 / 2.0) * dt;
        v2 = fv(x + x1, v + v1) * dt;
        x3 = fx(x + x2 / 2.0, v + v2 / 2.0) * dt;
        v3 = fv(x + x2 / 2.0, v + v2 / 2.0) * dt;
        x4 = fx(x + x3, v + v3) * dt;
        v4 = fv(x + x3, v + v3) * dt;
        x = x + (x1 + 2.0 * x2 + 2.0 * x3 + x4) / 6.0;
        v = v + (v1 + 2.0 * v2 + 3.0 * v3 + v4) / 6.0;
        ffstep += 1.0;
    }
    // last output should be
    // 9.99800 0.90782 0.19398
    // for both -g 0 and -g n (n > 0)
    printf("%.5f %.5f %.5f\n", time, x, v);
    float eps = 0.0001;
    if (aabs(time - 9.9980) > eps)
    {
        return 1;
    }
    if (aabs(x - 0.90782) > eps)
    {
        return 1;
    }

    if (aabs(v - 0.19398) > eps)
    {
        return 1;
    }

    return 0;
}

int run_even()
{
    float dt = 0.001;
    float x;
    float v;
    float x1;
    float v1;
    float x2;
    float v2;
    float x3;
    float v3;
    float x4;
    float v4;
    float ffstep = 0.0;
    float time;
    x = 0.0;
    v = -1.0;
    for (int step = 0; step < 10000; step++)
    {
        time = ffstep * dt;
        // printf("%.4f %.4f %.4f\n", time, x, v);
        x1 = fx(x, v) * dt;
        v1 = fv(x, v) * dt;
        x2 = fx(x + x1 / 2.0, v + v1 / 2.0) * dt;
        v2 = fv(x + x1, v + v1) * dt;
        x3 = fx(x + x2 / 2.0, v + v2 / 2.0) * dt;
        v3 = fv(x + x2 / 2.0, v + v2 / 2.0) * dt;
        x4 = fx(x + x3, v + v3) * dt;
        v4 = fv(x + x3, v + v3) * dt;
        x = x + (x1 + 2.0 * x2 + 2.0 * x3 + x4) / 6.0;
        v = v + (v1 + 2.0 * v2 + 3.0 * v3 + v4) / 6.0;
        ffstep += 1.0;
    }
    // last output should be
    // 9.99900 0.90801 0.19292
    // for both -g 0 and -g n (n > 0)
    printf("%.5f %.5f %.5f\n", time, x, v);
    float eps = 0.0001;
    if (aabs(time - 9.9990) > eps)
    {
        return 1;
    }
    if (aabs(x - 0.90801) > eps)
    {
        return 1;
    }

    if (aabs(v - 0.19292) > eps)
    {
        return 1;
    }

    return 0;
}

int main()
{
    if (run_odd() != 0)
    {
        return 1;
    }
    if (run_even() != 0)
    {
        return 1;
    }
    return 0;
}