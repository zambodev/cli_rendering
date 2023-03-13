#ifndef MAP_C
#define MAP_C

#include <stdio.h>
#include <math.h>
#include "vec2arr.h"

#define FOV 90
#define PI 3.14159265


void map_load(char map[], int32_t width, int32_t height, const char *filename)
{
	FILE *f = fopen(filename, "r");

	char str[width+2];

	for(int i=0; i<height; ++i)
	{
		fread(str, 1, width+2, f);
		strncpy(&map[i*width], str, width);
	}
	fclose(f);	
}

void map_print(char map[], int32_t width, int32_t height)
{
	printf("MAP VIEW:\n");
	printf("|-");
	for(int i=0; i<width; ++i)
		printf("--");
	printf("|\n");

	for(int i=0; i<height; ++i)
	{
		printf("| ");
		for(int j=0; j<width; ++j)
		{
			if(map[i*width+j] != '0')
				printf("%c ", map[i*width+j]);
			else
				printf("  ");	
		}
		printf("|");
		printf("\n");
	}
	
	printf("|-");
	for(int i=0; i<width; ++i)
		printf("--");
	printf("|\n");
}

/* Get visible pointes */
void map_getvp(char map[], int32_t width, int32_t height, vec2arr_t *points, float cam[2], float vangle)
{	
	for(float i=vangle+FOV/2; i>=vangle-FOV/2; i-=0.1)
	{
		float x=cam[0], y=cam[1];
		float tmp;
		do
		{
			x += cos(i*PI/180); 
			y -= sin(i*PI/180);
		}
		while(map[(int)y*width+(int)x] <= '1' && x >= 0 && x < width && y >= 0 && y < height);

		if(map[(int)y*width+(int)x] > '1' && map[(int)y*width+(int)x] < '9')
			vec2arr_insert(points, (int)x, (int)y, map[(int)y*width+(int)x]-48);	
	}
}

#endif