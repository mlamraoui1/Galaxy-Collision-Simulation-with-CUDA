#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

#include "GL/glew.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "text.h"

#define  SIZE_DISK_MILKYWAY 16384
#define  SIZE_DISK_ANDROMEDA 32768
#define  SIZE_BULGE_MILKYWAY 40960
#define  SIZE_BULGE_ANDROMEDA 49152
#define  SIZE_HALO_MILKYWAY 65536
#define  SIZE_HALO_ANDROMEDA 81920

#define  SIZE_UNIVERSE 1024
#define  MASSE 10
#define  XI 1
#define  FILENAME "./bin/dubinski.tab"
#define  DELTA_TIME 0.1f


typedef struct  {
	float x,y,z;
} Vector3f;

typedef struct  {
	float weight;
	Vector3f position;
	Vector3f velocity;
	Vector3f acceleration;
} Particle;


Particle universe[SIZE_UNIVERSE];

bool LoadFile(char * filename, int size){
	omp_set_num_threads( 4 );
	FILE* fd= fopen(filename, "r");
	char * t1 = NULL;
	size_t t2 = 0;
	if ( fd == NULL){
		printf("Unable to open file\n");
		return false;
	}
	else{
		printf("File opened successfully\n");
		int i, j;
		int step = floor(81920 / SIZE_UNIVERSE);
		for(i = 0; i < SIZE_UNIVERSE; i++)
		{
			if(fscanf(fd,"%f %f %f %f %f %f %f\n",&universe[i].weight,&universe[i].position.x,&universe[i].position.y,&universe[i].position.z,&universe[i].velocity.x,&universe[i].velocity.y,&universe[i].velocity.z)==EOF)
				break;
			for (j = 0; j < step; j++)
				getline(&t1, &t2, fd);
		}
		printf("File loaded successfully\n");	
	}
	fclose(fd);
	return true;
}

bool Simulation(){
	int i;	
	#pragma omp parallel for
	for (i = 0; i < SIZE_UNIVERSE; i++)
	{
		universe[i].acceleration.x = 0.0f;
		universe[i].acceleration.y = 0.0f; 
		universe[i].acceleration.z = 0.0f;
		int j;
		#pragma omp parallel for
		for (j = 0; j < SIZE_UNIVERSE; j++)
		{
			float Dx = universe[j].position.x - universe[i].position.x;
			float Dy = universe[j].position.y - universe[i].position.y;
			float Dz = universe[j].position.z - universe[i].position.z;
			float fact = 10 * universe[j].weight / dist;
			float sq = sqrtf(Dx * Dx + Dy * Dy + Dz * Dz);
			float dist = sq * sq * sq;

			if (dist < 1)
			{
				dist = 1;
			}

			universe[i].acceleration.x += Dx * fact;
			universe[i].acceleration.y += Dy * fact;
			universe[i].acceleration.z += Dz * fact;
		}
	}
	#pragma omp parallel for
	for (i = 0; i < SIZE_UNIVERSE; i++){
		universe[i].velocity.x += universe[i].acceleration.x;
		universe[i].velocity.y += universe[i].acceleration.y;
		universe[i].velocity.z += universe[i].acceleration.z;

		universe[i].position.x += universe[i].velocity.x * 0.1f;
		universe[i].position.y += universe[i].velocity.y * 0.1f;
		universe[i].position.z += universe[i].velocity.z * 0.1f;
	}
	

return true;	
}

	
