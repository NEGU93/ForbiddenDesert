#ifndef DEF_H_INCLUDED
#define DEF_H_INCLUDED	//Se incluyen definiciones y estructuras que usan todos los .cpp

#include <iostream>
#ifdef __cplusplus__
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#define HEIGHT 900
#define WIDTH 602
#define WHITE 255,255,255
#define BLACK 0,0,0
#define SIZEOFNAME 10
#define SECONDARY_BUTTON 2

typedef struct {
	int x;
	int y;
} pos;

using namespace std;

typedef unsigned char uchar;
//typedef unsigned long int uint;		// already on /usr/include/x86_64-linux-gnu/sys/types.h

#endif