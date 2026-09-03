#ifndef LIFE_H
#define LIFE_H

#ifndef H
#define H 300
#endif

#ifndef W
#define W 300
#endif

#ifndef F
#define F 300
#endif

extern float grid[H][W];
extern float ngrid[H][W];

void initBlob(void);
void initKernel(void);
float next(int xc, int yc);

#endif