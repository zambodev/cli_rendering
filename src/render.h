#ifndef RENDER_C
#define RENDER_C

#include <stdio.h>
#include <stdlib.h>
#include "vec2arr.h"
#include "map.h"


void render(vec2arr_t *points, int32_t width, int32_t height, float userp[2])
{
	char *matrix = calloc(points->size, 1);
	memset(matrix, '0', points->size);

	int max = -1;
	int min = points->height;
	for(int i=0; i<points->size; ++i)
	{
		if(i < points->fidx)
		{
			if(max < points->arr[i].y) 
				max = points->arr[i].y;
			else if(min > points->arr[i].y) 
				min = points->arr[i].y;
		}
	}

	int wall_w = points->fidx;
	const int inc = (points->height-wall_w)/2;
	int add = 0;
	for(int i=0; i<points->size; ++i)
	{
		if(i < points->fidx)
		{
			float d = sqrt((points->arr[i].x-userp[0])*(points->arr[i].x-userp[0]) + 
							(points->arr[i].y-userp[1])*(points->arr[i].y-userp[1]));

			//printf("%d %d %f\n", points->x[i], points->y[i], d);

			float tmp = 0;
			while(tmp < (points->height-d*1.5))
			{
				//printf("%f\n", tmp);
				//printf("%f %f\n", (SIZE-points->x[i])-tmp, tmp);
		
				if(i > 0 && points->arr[i].y > points->arr[i-1].y && points->arr[i].x > points->arr[i-1].x)
					add = sqrt(pow((points->arr[i].y - points->arr[i-1].y), 2) + pow((points->arr[i].x - points->arr[i-1].x), 2));

				matrix[(int)((points->height-d)-tmp)*points->height+inc+add+i] = points->arr[i].s+48;

				tmp += 1.0;
			}
		}

		//puts("---------");
	}

	map_print(matrix, width, height);

	free(matrix);
}

#endif