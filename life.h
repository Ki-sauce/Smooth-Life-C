#ifndef LIFE_H
#define LIFE_H

#include <stdio.h>

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

extern int ra;
extern int ri;

extern float b1;
extern float b2;
extern float d1;
extern float d2;
extern float an;
extern float am;

float s1(float x, float a, float alpha);
float s2(float x, float a, float b);
float sm(float x, float y, float m);
float s(float n, float m);

int emod(int a, int b);

float next(int xc, int yc);
void calcnext(void);
void setgrid(void);

void colorGray(float c, FILE *f);
void initPPM(FILE *f);
void initBlob(void);

#endif

