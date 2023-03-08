#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIZE 16
#define FOV 90


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

void vec2_print(vec2 *points)
{
	for(int i=0; i<SIZE; ++i)
		printf("%d %d\n", points->x[i], points->y[i]);	
	
	printf("\n");
}

void map_load(char map[], size_t size, const char *filename)
{
	FILE *f = fopen(filename, "r");
	fread(map, 1, size*size, f);
	fclose(f);	
}

void map_print(char map[], size_t size)
{
	for(int i=0; i<size; ++i)
	{
		for(int j=0; j<size; ++j)
		{
			printf("%c ", map[i*size+j]);
		}
		printf("\n");
	}

	printf("\n");

	fflush(stdout);
}

/* Get visible pointes */
void map_getvp(char map[], size_t size, vec2 *points)
{
	int upos[2];

	for(int i=0; i<size*size; ++i)
	{	
		if(map[i] == '1')
		{
			upos[0] = i/size;	// Y 
			upos[1] = i-((i/size)*size);	// X 
		}
	}
	
	double mul = 1;
	for(double i=FOV/2; i>=0; i-=0.2)
	{
		double x=upos[1], y=upos[0];
		double tmp;
		do
		{
			x -= mul*cos(i/2); 
			y -= sin(i/2);
			//printf("%f %f\n", x, y);
		}
		while(map[(int)y*size+(int)x] == '0' && x >= 0 && x < size && y >= 0 && y < size);
		
		if(map[(int)y*size+(int)x] == '2')
		{
			vec2_insert(points, (int)x, (int)y);	
			//printf("%d -- %dx %dy -- %c\n", (int)i, (int)x, (int)y, map[(int)y*size+(int)x]);
		}

		if(i == 0 && mul > 0)
		{
			i = FOV/2;
			mul = -1;
		}
	}

	
}

int main()
{

	char map[SIZE*SIZE];
	int arr[SIZE];
	vec2 points = {{0}, {0}, 0};

	map_load(map, SIZE, "map.txt");
	map_print(map, SIZE);
	map_getvp(map, SIZE, &points);
	vec2_print(&points);

	return 0;
}
