#include "cuda.h"
#include "kernel.cuh"
#include <stdio.h>
#include <math.h>

__device__ float4 Interaction(float4 p_i, float4 p_j, float4 a_i)
{
	float4 D;

	D.x = p_j.x - p_i.x;
	D.y = p_j.y - p_i.y;
	D.z = p_j.z - p_i.z;

	float dist = sqrtf(D.x * D.x + D.y * D.y + D.z * D.z);
	

	float dist3 = dist * dist * dist;

	if (dist3 < 1.0f) {
		dist3= 1.0f;
	}
	
	float fact = 10.0f * p_j.w / dist3;

	a_i.x += D.x * fact ;
	a_i.y += D.y * fact ;
	a_i.z += D.z * fact ;

	return a_i;
} 

__device__ float4 tile_calculation(float4 myPosition, float4 acc)
{
	extern __shared__ float4 shPosition[];

	for (unsigned int i = 0; i < BSIZE; i++)
		acc = Interaction(myPosition, shPosition[i], acc);

	return acc;
}


__global__ void galaxyKernel(float4* pos, float4 * pdata, unsigned int width, unsigned int height)
{

	// p bodysdescriptions from the shared memory
	extern __shared__ float4 shPosition[];









__global__ void kernel_saxpy( int n, float a, float * x, float * y, float * z ) {
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if ( i < n ) { 
		z[i] = a * x[i] + y [i];
	}
}

void saxpy( int nblocks, int nthreads, int n, float a, float * x, float * y, float * z ) {
	kernel_saxpy<<<nblocks, nthreads>>>( n, a, x, y, z );
}
