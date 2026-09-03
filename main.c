#include "life.h"
#include "barrier.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 8

thread_barrier_t barrier_start;
thread_barrier_t barrier_end;

typedef struct {
    int start_y;
    int end_y;
} ThreadArgs;

void* calcnext_worker(void* arg)
{
    ThreadArgs* args = (ThreadArgs*)arg;

    for (int frame = 0; frame < F; ++frame)
    {
        barrier_wait(&barrier_start);

        for (int y = args->start_y; y < args->end_y; ++y)
        {
            for (int x = 0; x < W; ++x)
            {
                ngrid[y][x] = next(x, y);
            }
        }

        barrier_wait(&barrier_end);
    }
    return NULL;
}

int main(void)
{
    char filename[64];

    initBlob();
    initKernel();

    barrier_init(&barrier_start, NUM_THREADS + 1);
    barrier_init(&barrier_end, NUM_THREADS + 1);

    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];
    int chunk_size = H / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        args[i].start_y = i * chunk_size;
        args[i].end_y = (i == NUM_THREADS - 1) ? H : (i + 1) * chunk_size;
        
        pthread_create(&threads[i], NULL, calcnext_worker, &args[i]);
    }

    // Allocate flat image buffer
    unsigned char *img_buffer = (unsigned char *)malloc(W * H * 3);
    if (!img_buffer)
    {
        fprintf(stderr, "Failed to allocate image buffer\n");
        return 1;
    }

    for (int frame = 0; frame < F; ++frame)
    {
        snprintf(filename, sizeof(filename), "output_%03d.ppm", frame);
        FILE *f = fopen(filename, "wb");
        
        if (!f)
        {
            perror(filename);
            return 1;
        }

        // Inline initPPM to write directly to the file stream
        fprintf(f, "P6\n%d %d\n255\n", W, H);

        int idx = 0;
        for (int y = 0; y < H; ++y)
        {
            for (int x = 0; x < W; ++x)
            {
                float c = grid[y][x];
                if (c < 0.0f) c = 0.0f;
                if (c > 1.0f) c = 1.0f;
                
                unsigned char val = (unsigned char)(255.0f * c);

                img_buffer[idx++] = val;
                img_buffer[idx++] = val;
                img_buffer[idx++] = val;
            }
        }

        fwrite(img_buffer, 1, W * H * 3, f);
        fclose(f);

        // Sync: Execute frame calculations
        barrier_wait(&barrier_start);
        barrier_wait(&barrier_end);

        // Fast state swap
        memcpy(grid, ngrid, sizeof(grid));
    }

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    barrier_destroy(&barrier_start);
    barrier_destroy(&barrier_end);
    free(img_buffer);

    return 0;
}