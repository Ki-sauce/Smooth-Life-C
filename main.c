#include "life.h"

#include <stdio.h>

int main(void)
{
    char filename[64];

    initBlob();

    for (int frame = 0; frame < F; ++frame)
    {
        snprintf(
            filename,
            sizeof(filename),
            "output_%03d.ppm",
            frame
        );

        FILE *f = fopen(filename, "wb");

        if (!f)
        {
            perror(filename);
            return 1;
        }

        initPPM(f);

        for (int y = 0; y < H; ++y)
        {
            for (int x = 0; x < W; ++x)
            {
                colorGray(grid[y][x], f);
            }
        }

        calcnext();
        setgrid();

        fclose(f);
    }

    return 0;
}

