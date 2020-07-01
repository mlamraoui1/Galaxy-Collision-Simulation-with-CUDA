#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>

#include "GL/glew.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "text.h"

#define  SIZE_BULGE 8192
#define  SIZE_DISK 16384
#define  SIZE_HALO 16384
#define  SIZE_UNIVERSE 81920


typedef struct  {
	float x,y,z;
} Vector3f;



typedef struct  {
	float weight;
	Vector3f position;
	Vector3f velocity;
} Particle;



Particle *universe;
bool LoadFile( char * filename, int size){
	FILE* fd= fopen(filename, "r");
	if ( fd==NULL){
		printf("Unable to open file\n");
		return false;
	}
	else{
		printf("File opened successfully\n");
		universe = NULL;
		do {
		universe= (Particle*)malloc(size*sizeof(Particle));
		} while(universe == NULL);

		int i;
		for(i=0;i<SIZE_UNIVERSE;i++){
			if(fscanf(fd,"%f %f %f %f %f %f %f\n",&universe[i].weight,&universe[i].position.x,&universe[i].position.y,&universe[i].position.z,&universe[i].velocity.x,&universe[i].velocity.y,&universe[i].velocity.z)==EOF){
			break;
			}

		}

	printf("File loaded successfully\n");
	
	}
	free(universe);
	fclose(fd);
	return true;
}
	
