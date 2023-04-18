#ifndef RENDER_C
#define RENDER_C

#include <stdio.h>
#include <stdlib.h>
#include "vec2arr.h"
#include "map.h"


const char shadow[5] = {'#', '/', ':', '.', ' '};

void render(vec2arr_t *points, int32_t width, int32_t height, float userp[2])
{
	char *matrix = calloc(width*height, 1);
	memset(matrix, '0', width*height);
	int j = 0;
	float sect, inc;
	sect = inc = (int)(width/points->fidx);

	for(int i = 0; i < width; ++i)
	{
		if(i > sect)
		{
			++j;
			sect += inc;
			//printf("%d %f\n", j, sect);
		}

		float d = sqrt((points->arr[j].x - userp[0]) * (points->arr[j].x - userp[0]) + 
						(points->arr[j].y - userp[1]) * (points->arr[j].y - userp[1]));

		//printf("%d %d %f\n", points->x[i], points->y[i], d);
		
		float tmp = 0;
		while(tmp < (height - d * 1.5))
		{
			//printf("%f\n", tmp);
			//printf("%f %f\n", (SIZE-points->x[i])-tmp, tmp);

			matrix[(int)(height - (d + tmp)) * height + i] = '#';

			tmp += 1.0;
		}

		//puts("---------");
	}

	map_print(matrix, width, height);

	free(matrix);
}

#endif
