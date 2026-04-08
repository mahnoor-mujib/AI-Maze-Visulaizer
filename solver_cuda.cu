#include <stdio.h>
#include <cuda_runtime.h>

__global__ void dummyKernel() {
    printf("GPU Kernel Running!\n");
}

// 👇 IMPORTANT: no extern "C"
void runCUDA() {
    dummyKernel<<<1,1>>>();
    cudaDeviceSynchronize();
}