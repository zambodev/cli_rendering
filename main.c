#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define SIZE 16
#define FOV 90
#define PI 3.14159265

typedef struct VEC2
{
	int x[SIZE];
	int y[SIZE];
	int fill_idx;
} vec2;

int vec2_find(vec2 *points, int x, int y)
{
	for(int i=0; i<SIZE; ++i)
		if(points->x[i] == x && points->y[i] == y)
			return 1;

	return 0;
}

void vec2_insert(vec2 *points, int x, int y)
{
	if(vec2_find(points, x, y) || points->fill_idx == SIZE) return;

	points->x[points->fill_idx] = x;	
	points->y[points->fill_idx] = y;	

	++points->fill_idx;
}

void vec2_print(vec2 *points, size_t size, int setgraph)
{	
	if(setgraph)
	{
		printf("POINT VIEW:\n");
		printf("|-");
		for(int i=0; i<size; ++i)
			printf("--");
		printf("|\n");

		for(int i=0; i<size; ++i)
		{
			printf("| ");
			for(int j=0; j<size; ++j)
			{	
				if(vec2_find(points, j, i))
					printf("2 ");
				else
					printf("  ");
			}
			printf("|");
			printf("\n");
		}
		
		printf("|-");
		for(int i=0; i<size; ++i)
			printf("--");
		printf("|\n");
	}
	else
	{
		for(int i=0; i<SIZE; ++i)
			printf("%d %d\n", points->x[i], points->y[i]);	
	}

	printf("\n");
}

void map_load(char map[], size_t size, const char *filename)
{
	FILE *f = fopen(filename, "r");
	fread(map, 1, size*size, f);
	fclose(f);	
}

void map_print(char map[], size_t size, int setzero)
{
	printf("MAP VIEW:\n");
	printf("|-");
	for(int i=0; i<size; ++i)
		printf("--");
	printf("|\n");

	for(int i=0; i<size; ++i)
	{
		printf("| ");
		for(int j=0; j<size; ++j)
		{
			if(setzero || map[i*size+j] != '0')
				printf("%c ", map[i*size+j]);
			else
				printf("  ");	
		}
		printf("|");
		printf("\n");
	}
	
	printf("|-");
	for(int i=0; i<size; ++i)
		printf("--");
	printf("|\n");
}

/* Get visible pointes */
void map_getvp(char map[], size_t size, vec2 *points, double cam[2], double vangle)
{
	for(int i=0; i<size*size; ++i)
	{	
		if(map[i] == '1')
		{
			cam[0] = i-((i/size)*size);	// X 
			cam[1] = i/size;	// Y 
		}
	}
	
	for(double i=vangle+FOV/2; i>=vangle-FOV/2; i-=0.1)
	{
		double x=cam[0], y=cam[1];
		double tmp;
		do
		{
			x += cos(i*PI/180); 
			y -= sin(i*PI/180);
		}
		while(map[(int)y*size+(int)x] == '0' && x >= 0 && x < size && y >= 0 && y < size);

		if(map[(int)y*size+(int)x] == '2')
		{
			vec2_insert(points, (int)x, (int)y);	
			//printf("%d -- %fx %fy -- %c\n", (int)i, x, y, map[(int)y*size+(int)x]);
		}

	}

	
}

void render(vec2 *points, size_t size, double userp[2])
{
	char *matrix = calloc(size*size, 1);
	memset(matrix, '0', size*size);

	for(int i=0; i<size; ++i)
	{
		if(i < points->fill_idx)
		{
			double d = sqrt((points->x[i]-userp[0])*(points->x[i]-userp[0]) + 
							(points->y[i]-userp[1])*(points->y[i]-userp[1]));

			//printf("%d %d\n", points->x[i], points->y[i]);

			double tmp = 0;
			while(tmp < d && (size-points->x[i])-tmp >= 0)
			{
				//printf("%f %f\n", (size-points->x[i])-tmp, tmp);
				matrix[(int)((size-points->x[i])-tmp)*size+points->y[i]] = '2';

				tmp += 1.0;
			}
		}
		//puts("---------");
	}

	map_print(matrix, size, 0);

	free(matrix);
}

int main(int argc, char **argv)
{
	char map[SIZE*SIZE];
	double camera[2];
	vec2 points;

	memset(points.x, -1, SIZE*4);
	memset(points.y, -1, SIZE*4);
	points.fill_idx = 0;

	map_load(map, SIZE, "map.txt");

	double angle = strtod(argv[1], NULL);
	double val = 1;

#ifdef SHOWOFF
	while(1)
	{
		usleep(500000);
		printf("\x1b[H\x1b[2J");
		fflush(stdout);
		printf("%f\n", angle);
		memset(points.x, -1, SIZE*4);
		memset(points.y, -1, SIZE*4);
		points.fill_idx = 0;
#endif

		map_print(map, SIZE, 0);
		map_getvp(map, SIZE, &points, camera, angle);
		vec2_print(&points, SIZE, 1);
		render(&points, SIZE, camera);

#ifdef SHOWOFF
		angle -= val;
		if(angle<0.0) val *= -1;
	}
#endif

	return 0;
}
