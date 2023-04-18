#ifndef VEC2ARR_C
#define VEC2ARR_C

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


typedef struct VEC2_T
{
	int32_t x;
	int32_t y;
	int32_t s;
} vec2_t;

typedef struct VEC2ARRAY_T
{
	int32_t width;
	int32_t height;
	int32_t size;
	int32_t fidx; 
	vec2_t *arr;
} vec2arr_t;


int32_t vec2arr_init(vec2arr_t *vecarr, int32_t width, int32_t height)
{
	if(width < 0 || height < 0) return -1;

	vecarr->width = width;
	vecarr->height = height;
	vecarr->size = width*height;
	vecarr->fidx = 0;

	vecarr->arr = (vec2_t *)malloc(sizeof(vec2_t)*vecarr->size);
	memset(vecarr->arr, -1, sizeof(vec2_t)*vecarr->size);

	return 0;
}

void vec2arr_free(vec2arr_t *vecarr)
{
	memset(vecarr->arr, 0, sizeof(vec2_t)*vecarr->size);
	free(vecarr->arr);

	return;
}

int32_t vec2arr_find(vec2arr_t *points, int x, int y)
{
	for(int i=0; i<points->size; ++i)
		if(points->arr[i].x == x && points->arr[i].y == y)
			return points->arr[i].s;

	return 0;
}

int32_t vec2arr_insert(vec2arr_t *points, int32_t x, int32_t y, int32_t s)
{
	if(vec2arr_find(points, x, y) || points->fidx == points->size) return -1;

	points->arr[points->fidx].x = x;	
	points->arr[points->fidx].y = y;
	points->arr[points->fidx].s = s;

	++points->fidx;

	return 0;
}

void vec2arr_print(vec2arr_t *points, float cam[2], int32_t graph)
{	
	if(graph)
	{
		/* Print camera point */
		printf("\x1b[s\x1b[%d;%dH\x1b[u", (int32_t)cam[0], (int32_t)cam[1]);

		/* Top side & title */
		printf("POINT VIEW:\n");
		printf("|-");
		for(int i=0; i<points->width; ++i)
			printf("--");
		printf("|\n");

		/* Matrix with l&r sides */
		for(int i=0; i<points->height; ++i)
		{
			printf("| ");
			for(int j=0; j<points->width; ++j)
			{	
				int tmp;
				if((tmp = vec2arr_find(points, j, i)))
					printf("%d ", tmp);
				else
					printf("  ");
			}
			printf("|");
			printf("\n");
		}
		
		/* Bottom side  */
		printf("|-");
		for(int i=0; i<points->width; ++i)
			printf("--");
		printf("|\n");
	}
	else
	{
		for(int i=0; i<points->fidx; ++i)
			printf("%d %d\n", points->arr[i].x, points->arr[i].y);	
	}

	printf("\n");
}

#endif
