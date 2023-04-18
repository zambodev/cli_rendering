#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "vec2arr.h"
#include "map.h"
#include "render.h"


int main(int argc, char **argv)
{
	int32_t w, h;
	float camera[2];
	char *map;
	vec2arr_t vecarr;

	w = (int)strtol(argv[1], NULL, 10);
	h = (int)strtol(argv[2], NULL, 10);
	map = (char *)malloc(w*h);

	vec2arr_init(&vecarr, w, h);
	map_load(map, w, h, "map.txt");

	for(int i=0; i<w*h; ++i)
	{	
		if(map[i] == '1')
		{
			camera[0] = i-((i/w)*w);	// X 
			camera[1] = i/w;	// Y 
		}
	}

	float angle = strtof(argv[3], NULL);

#ifdef SHOWOFF
	int val = 1;

	while(1)
	{		
		camera[1] -= val;
		angle -= val*4;

		if(camera[1] == 1)
			val *= -1;
#endif

	map_getvp(map, w, h, &vecarr, camera, angle);
#ifndef SHOWOFF
	map_print(map, w, h);
	vec2arr_print(&vecarr, camera, 1);
#endif
	render(&vecarr, w, h, camera);

#ifdef SHOWOFF
		camera[1] -= val;
		angle -= val*4;

		if(camera[1] == 1)
			val *= -1;

		usleep(500000);
	}
#endif

	free(map);
	vec2arr_free(&vecarr);

	return 0;
}