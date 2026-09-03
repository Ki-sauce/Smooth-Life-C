#include "life.h"

#include <math.h>
#include <stdlib.h>
#include <time.h>

float grid[H][W] = {{0.0f}};
float ngrid[H][W] = {{0.0f}};

//static parameters cause why not
static int ra = 21;
static int ri = 7;

static float b1 = 0.278f;
static float b2 = 0.365f;
static float d1 = 0.267f;
static float d2 = 0.445f;
static float an = 0.028f;
static float am = 0.147f;

// The precalculated distance kernel
typedef struct {
    int dx, dy;
    float w_m, w_n;
} KernelNode;

static KernelNode kernel[4000];
static int kernel_size = 0;
static float total_M = 0.0f;
static float total_N = 0.0f;

static float s1(float x, float a, float alpha)
{
    return 1.0f / (1.0f + expf(-(x - a) * 4.0f / alpha));
}

static float s2(float x, float a, float b)
{
    return s1(x, a, an) * (1.0f - s1(x, b, an));
}

static float sm(float x, float y, float m)
{
    float t = s1(m, 0.5f, am);
    return x * (1.0f - t) + y * t;
}

static float s(float n, float m)
{
    return s2(
        n,
        sm(b1, d1, m),
        sm(b2, d2, m)
    );
}

static int emod(int a, int b)
{
    return (a % b + b) % b;
}

void initKernel(void)
{
    for (int dx = -ra; dx <= ra; ++dx)
    {
        for (int dy = -ra; dy <= ra; ++dy)
        {
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

            if (w_m > 0.0f || w_n > 0.0f)
            {
                kernel[kernel_size].dx = dx;
                kernel[kernel_size].dy = dy;
                kernel[kernel_size].w_m = w_m;
                kernel[kernel_size].w_n = w_n;

                total_M += w_m;
                total_N += w_n;
                kernel_size++;
            }
        }
    }
}

float next(int xc, int yc)
{
    float m = 0.0f;
    float n = 0.0f;

    // Linear pass over the precalculated kernel
    for (int i = 0; i < kernel_size; ++i)
    {
        int x = emod(xc + kernel[i].dx, W);
        int y = emod(yc + kernel[i].dy, H);

        float value = grid[y][x];

        m += value * kernel[i].w_m;
        n += value * kernel[i].w_n;
    }

    return s(n / total_N, m / total_M);
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

    srand((unsigned int)time(NULL));

    // Create between 10 and 25 random circular clusters of noise
    int num_splatters = 10 + (rand() % 16);

    for (int i = 0; i < num_splatters; ++i)
    {
        // Pick a random center for the splatter
        int cx = rand() % W;
        int cy = rand() % H;
        
        // Pick a random radius between 10 and 35 pixels
        int radius = 10 + (rand() % 26); 

        for (int dy = -radius; dy <= radius; ++dy)
        {
            for (int dx = -radius; dx <= radius; ++dx)
            {
                // Only fill pixels inside the circle
                if (dx * dx + dy * dy <= radius * radius)
                {
                    int x = emod(cx + dx, W);
                    int y = emod(cy + dy, H);
                    
                    // Assign a completely random fractional value (0.0 to 1.0)
                    // instead of a binary solid 1.0f.
                    grid[y][x] = (float)rand() / (float)RAND_MAX;
                }
            }
        }
    }
}