#include "life.h"

#include <math.h>
#include <stdlib.h>
#include <time.h>

float grid[H][W] = {0.0f};
float ngrid[H][W] = {0.0f};

int ra = 21;
int ri = 7;

float b1 = 0.278f;
float b2 = 0.365f;
float d1 = 0.267f;
float d2 = 0.445f;
float an = 0.028f;
float am = 0.147f;

float s1(float x, float a, float alpha)
{
    return 1.0f / (1.0f + expf(-(x - a) * 4.0f / alpha));
}

float s2(float x, float a, float b)
{
    return s1(x, a, an) * (1.0f - s1(x, b, an));
}

float sm(float x, float y, float m)
{
    float t = s1(m, 0.5f, am);

    return x * (1.0f - t) + y * t;
}

float s(float n, float m)
{
    return s2(
        n,
        sm(b1, d1, m),
        sm(b2, d2, m)
    );
}

int emod(int a, int b)
{
    return (a % b + b) % b;
}

float next(int xc, int yc)
{
    float m = 0.0f;
    float n = 0.0f;
    float M = 0.0f;
    float N = 0.0f;

    for (int dx = -ra; dx <= ra; ++dx)
    {
        for (int dy = -ra; dy <= ra; ++dy)
        {
            int x = emod(xc + dx, W);
            int y = emod(yc + dy, H);

            float r = sqrtf((float)(dx * dx + dy * dy));

            float w_m;

            if (r < ri - 0.5f)
                w_m = 1.0f;
            else if (r < ri + 0.5f)
                w_m = (ri + 0.5f) - r;
            else
                w_m = 0.0f;

            float w_a;

            if (r < ra - 0.5f)
                w_a = 1.0f;
            else if (r < ra + 0.5f)
                w_a = (ra + 0.5f) - r;
            else
                w_a = 0.0f;

            float w_n = w_a - w_m;

            if (w_n < 0.0f)
                w_n = 0.0f;

            float value = grid[y][x];

            m += value * w_m;
            M += w_m;

            n += value * w_n;
            N += w_n;
        }
    }

    return s(n / N, m / M);
}

void calcnext(void)
{
    for (int y = 0; y < H; ++y)
    {
        for (int x = 0; x < W; ++x)
        {
            ngrid[y][x] = next(x, y);
        }
    }
}

void setgrid(void)
{
    for (int y = 0; y < H; ++y)
    {
        for (int x = 0; x < W; ++x)
        {
            grid[y][x] = ngrid[y][x];
        }
    }
}

void colorGray(float c, FILE *f)
{
    if (c < 0.0f)
        c = 0.0f;

    if (c > 1.0f)
        c = 1.0f;

    unsigned char value = (unsigned char)(255.0f * c);

    fputc(value, f);
    fputc(value, f);
    fputc(value, f);
}

void initPPM(FILE *f)
{
    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", W, H);
    fprintf(f, "255\n");
}

void initBlob(void)
{
    for (int y = 0; y < H; ++y)
    {
        for (int x = 0; x < W; ++x)
        {
            grid[y][x] = 0.0f;
        }
    }

    int cx = W / 2;
    int cy = H / 2;

    srand((unsigned int)time(NULL));

    for (int y = cy - 40; y < cy + 40; y += 10)
    {
        for (int x = cx - 40; x < cx + 40; x += 10)
        {
            if ((float)rand() / (float)RAND_MAX > 0.5f)
            {
                for (int dy = 0; dy < 10; ++dy)
                {
                    for (int dx = 0; dx < 10; ++dx)
                    {
                        grid[y + dy][x + dx] = 1.0f;
                    }
                }
            }
        }
    }
}

