#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define SIZE 32
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

void vec2_print(vec2 *points, float cam[2], int setgraph)
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
				else if((int)cam[0] == j && (int)cam[1] == i)
					printf("1 ");
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

	char str[SIZE+2];

	for(int i=0; i<SIZE; ++i)
	{
		fread(str, 1, SIZE+2, f);
		strncpy(&map[i*SIZE], str, SIZE);
	}
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
void map_getvp(char map[], vec2 *points, float cam[2], float vangle)
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
		while(map[(int)y*SIZE+(int)x] <= '1' && x >= 0 && x < SIZE && y >= 0 && y < SIZE);

		if(map[(int)y*SIZE+(int)x] > '1' && map[(int)y*SIZE+(int)x] < '9')
		{
			vec2_insert(points, (int)x, (int)y, map[(int)y*SIZE+(int)x]-48);	
			//printf("%.2f -- %.2fx %.2fy -- %c %d\n", i, x, y, map[(int)y*SIZE+(int)x], (int)y);
		}

	}

	
}

void render(vec2 *points, float userp[2])
{
	char *matrix = calloc(SIZE*SIZE, 1);
	memset(matrix, '0', SIZE*SIZE);

	int max = -1;
	int min = SIZE;
	for(int i=0; i<SIZE*SIZE; ++i)
	{
		if(i < points->f)
		{
			if(max < points->y[i]) 
				max = points->y[i];
			else if(min > points->y[i]) 
				min = points->y[i];
		}
	}

	int width = points->f;
	const int inc = (SIZE-width)/2;
	int add = 0;
	for(int i=0; i<SIZE*SIZE; ++i)
	{
		if(i < points->f)
		{
			float d = sqrt((points->x[i]-userp[0])*(points->x[i]-userp[0]) + 
							(points->y[i]-userp[1])*(points->y[i]-userp[1]));

			//printf("%d %d %f\n", points->x[i], points->y[i], d);

			float tmp = 0;
			while(tmp < (SIZE-d*1.5))
			{
				//printf("%f\n", tmp);
				//printf("%f %f\n", (SIZE-points->x[i])-tmp, tmp);
		
				if(i > 0 && points->y[i] > points->y[i-1] && points->x[i] > points->x[i-1])
					add = sqrt(pow((points->y[i] - points->y[i-1]), 2) + pow((points->x[i] - points->x[i-1]), 2));

				matrix[(int)((SIZE-d)-tmp)*SIZE+inc+add+i] = points->h[i]+48;

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
	float camera[2];
	vec2 points;

	memset(points.x, -1, SIZE*SIZE*4);
	memset(points.y, -1, SIZE*SIZE*4);
	memset(points.h, 0, SIZE*SIZE*4);
	points.f = 0;

	map_load(map, "map.txt");

	for(int i=0; i<SIZE*SIZE; ++i)
	{	
		if(map[i] == '1')
		{
			camera[0] = i-((i/SIZE)*SIZE);	// X 
			camera[1] = i/SIZE;	// Y 
		}
	}

	float angle = strtod(argv[1], NULL);
	float val = 1;

#ifdef SHOWOFF
	while(1)
	{
		usleep(500000);
		printf("\x1b[H\x1b[2J");
		fflush(stdout);
		memset(points.x, -1, SIZE*SIZE*4);
		memset(points.y, -1, SIZE*SIZE*4);
		memset(points.h, 0, SIZE*SIZE*4);
		points.f = 0;
#endif

		map_print(map, 0);
		map_getvp(map, &points, camera, angle);
		vec2_print(&points, camera, 1);
		render(&points, camera);

#ifdef SHOWOFF
		camera[1] -= val;
		angle -= val*4;

		if(camera[1] == 1)
			val *= -1;
		
	}
#endif

	return 0;
}
