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
	int x[SIZE*SIZE];
	int y[SIZE*SIZE];
	int h[SIZE*SIZE];
	int f;
} vec2;

int vec2_find(vec2 *points, int x, int y)
{
	for(int i=0; i<SIZE*SIZE; ++i)
		if(points->x[i] == x && points->y[i] == y)
			return points->h[i];

	return 0;
}

void vec2_insert(vec2 *points, int x, int y, int h)
{
	if(vec2_find(points, x, y) || points->f == SIZE*SIZE) return;

	points->x[points->f] = x;	
	points->y[points->f] = y;
	points->h[points->f] = h;	

	++points->f;
}

void vec2_print(vec2 *points, int setgraph)
{	
	if(setgraph)
	{
		printf("POINT VIEW:\n");
		printf("|-");
		for(int i=0; i<SIZE; ++i)
			printf("--");
		printf("|\n");

		for(int i=0; i<SIZE; ++i)
		{
			printf("| ");
			for(int j=0; j<SIZE; ++j)
			{	
				int tmp;
				if((tmp = vec2_find(points, j, i)))
					printf("%d ", tmp);
				else
					printf("  ");
			}
			printf("|");
			printf("\n");
		}
		
		printf("|-");
		for(int i=0; i<SIZE; ++i)
			printf("--");
		printf("|\n");
	}
	else
	{
		for(int i=0; i<SIZE*SIZE; ++i)
			printf("%d %d\n", points->x[i], points->y[i]);	
	}

	printf("\n");
}

void map_load(char map[], const char *filename)
{
	FILE *f = fopen(filename, "r");
	fread(map, 1, SIZE*SIZE, f);
	fclose(f);	
}

void map_print(char map[], int setzero)
{
	printf("MAP VIEW:\n");
	printf("|-");
	for(int i=0; i<SIZE; ++i)
		printf("--");
	printf("|\n");

	for(int i=0; i<SIZE; ++i)
	{
		printf("| ");
		for(int j=0; j<SIZE; ++j)
		{
			if(setzero || map[i*SIZE+j] != '0')
				printf("%c ", map[i*SIZE+j]);
			else
				printf("  ");	
		}
		printf("|");
		printf("\n");
	}
	
	printf("|-");
	for(int i=0; i<SIZE; ++i)
		printf("--");
	printf("|\n");
}

/* Get visible pointes */
void map_getvp(char map[], vec2 *points, double cam[2], double vangle)
{
	for(int i=0; i<SIZE*SIZE; ++i)
	{	
		if(map[i] == '1')
		{
			cam[0] = i-((i/SIZE)*SIZE);	// X 
			cam[1] = i/SIZE;	// Y 
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
		while(map[(int)y*SIZE+(int)x] <= '1' && x >= 0 && x < SIZE && y >= 0 && y < SIZE);

		if(map[(int)y*SIZE+(int)x] > '1' && map[(int)y*SIZE+(int)x] < '9')
		{
			vec2_insert(points, (int)x, (int)y, map[(int)y*SIZE+(int)x]-48);	
			//printf("%.2f -- %.2fx %.2fy -- %c\n", i, x, y, map[(int)y*SIZE+(int)x]);
		}

	}

	
}

void render(vec2 *points, double userp[2])
{
	char *matrix = calloc(SIZE*SIZE, 1);
	memset(matrix, '0', SIZE*SIZE);

	for(int i=0; i<SIZE; ++i)
	{
		if(i < points->f)
		{
			double d = sqrt((points->x[i]-userp[0])*(points->x[i]-userp[0]) + 
							(points->y[i]-userp[1])*(points->y[i]-userp[1]));

			//printf("%d %d\n", points->x[i], points->y[i]);

			double tmp = 0;
			while(tmp < d && (SIZE-points->x[i])-tmp >= 0)
			{
				//printf("%f %f\n", (SIZE-points->x[i])-tmp, tmp);
				matrix[(int)((SIZE-points->x[i])-tmp)*SIZE+points->y[i]] = points->h[i];

				tmp += 1.0;
			}
		}
		//puts("---------");
	}

	map_print(matrix, 0);

	free(matrix);
}

int main(int argc, char **argv)
{
	char map[SIZE*SIZE];
	double camera[2];
	vec2 points;

	memset(points.x, -1, SIZE*SIZE*4);
	memset(points.y, -1, SIZE*SIZE*4);
	memset(points.h, 0, SIZE*SIZE*4);
	points.f = 0;

	map_load(map, "map.txt");

	double angle = strtod(argv[1], NULL);
	double val = 1;

#ifdef SHOWOFF
	while(1)
	{
		usleep(500000);
		printf("\x1b[H\x1b[2J");
		fflush(stdout);
		printf("%f\n", angle);
		memset(points.x, -1, SIZE*SIZE*4);
		memset(points.y, -1, SIZE*SIZE*4);
		memset(points.h, 0, SIZE*SIZE*4);
		points.f = 0;
#endif

		map_print(map, 0);
		map_getvp(map, &points, camera, angle);
		vec2_print(&points, 1);
		//render(&points, camera);

#ifdef SHOWOFF
		angle -= val;
		if(angle<0.0) val *= -1;
	}
#endif

	return 0;
}
