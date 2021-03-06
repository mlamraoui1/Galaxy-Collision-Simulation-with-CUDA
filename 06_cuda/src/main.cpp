#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cuda_runtime.h"
#include "kernel.cuh"

//#define VERBOSE

inline bool CUDA_MALLOC( void ** devPtr, size_t size ) {
	cudaError_t cudaStatus;
	cudaStatus = cudaMalloc( devPtr, size );
	if ( cudaStatus != cudaSuccess ) {
		printf( "error: unable to allocate buffer\n");
		return false;
	}
	return true;
}

inline bool CUDA_MEMCPY( void * dst, const void * src, size_t count, enum cudaMemcpyKind kind ) {
	cudaError_t cudaStatus;
	cudaStatus = cudaMemcpy( dst, src, count, kind );
	if ( cudaStatus != cudaSuccess ) {
		printf( "error: unable to copy buffer\n");
		return false;
	}
	return true;
}
	
void RandomizeFloatArray( int n, float * arr ) {
	for ( int i = 0; i < n; i++ ) {
		arr[i] = (float)rand() / ( (float)RAND_MAX / 2.0f ) - 1.0f;
	}
}

int main( int argc, char ** argv ) {
	
	if ( argc != 3 ) {
		printf( "usage: cuda vector_size num_threads\n" );
		return 0;
	}
	
	int n = atoi( argv[1] );
	int numThreads = atoi( argv[2] );

	float * hostSrc1 = NULL;
	float * hostSrc2 = NULL;
	float * hostDst1 = NULL;

	float * deviceSrc1 = NULL;
	float * deviceSrc2 = NULL;
	float * deviceDst1 = NULL;

	srand( (unsigned int)time( NULL ) );

	hostSrc1 = (float *)malloc( sizeof( float ) * n );
	hostSrc2 = (float *)malloc( sizeof( float ) * n );
	hostDst1 = (float *)malloc( sizeof( float ) * n );

	RandomizeFloatArray( n, hostSrc1 );
	RandomizeFloatArray( n, hostSrc2 );

	memset( hostDst1, 0.0f, sizeof( float ) * n );

	cudaError_t cudaStatus;

	cudaStatus = cudaSetDevice( 0 );

	if ( cudaStatus != cudaSuccess ) {
		printf( "error: unable to setup cuda device\n");
	}
	
	CUDA_MALLOC( (void**)&deviceSrc1, n * sizeof( float ) );
	CUDA_MALLOC( (void**)&deviceSrc2, n * sizeof( float ) );
	CUDA_MALLOC( (void**)&deviceDst1, n * sizeof( float ) );

	CUDA_MEMCPY( deviceSrc1, hostSrc1, n * sizeof( float ), cudaMemcpyHostToDevice );
	CUDA_MEMCPY( deviceSrc2, hostSrc2, n * sizeof( float ), cudaMemcpyHostToDevice );

	int numBlocks = ( n + ( numThreads - 1 ) ) / numThreads;
	//Process
	saxpy( numBlocks, numThreads, n, 2.0, deviceSrc1, deviceSrc2, deviceDst1 );
	
	cudaStatus = cudaDeviceSynchronize();
	
	if ( cudaStatus != cudaSuccess ) {
		printf( "error: unable to synchronize threads\n");
	}
	//Copy result to Host
	CUDA_MEMCPY( hostDst1, deviceDst1, n * sizeof( float ), cudaMemcpyDeviceToHost );
	
	#ifdef VERBOSE
		for ( int i = 0; i < n; i++ ) {
			printf( "%5.2f * %5.2f + %5.2f = %5.2f\n",  2.0f, hostSrc1[i], hostSrc2[i], hostDst1[i] );
		}
	#endif
	
	cudaStatus = cudaDeviceReset();

	if (cudaStatus != cudaSuccess) {
		printf( "(EE) Unable to reset device\n" );
	}
	
	printf( "done\n" );

	return 1;

}
