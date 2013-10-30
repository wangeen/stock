#include <stdio.h>
//#include <cutil.h>
#include "coefficient.h"

#define THREAD_NUM 8
#define BLOCK_NUM 1
#define ARRAY_LENGTH PRICE_LIST_SIZE
#define BLOCK_SIZE 8
const int threadsPerBlock = 256; 

static void HandleError( cudaError_t err, const char *file, int line ) { 
    if (err != cudaSuccess) {
        printf( "%s in %s at line %d\n",  cudaGetErrorString( err ), 
                file,  line ); 
        exit( EXIT_FAILURE ); 
    }
}

#define HANDLE_ERROR( err ) (HandleError( err,  __FILE__,  __LINE__ ))
#define HANDLE_NULL( a ) {if (a  ==  NULL) { \
                printf( "Host memory failed in %s at line %d\n",  \
                __FILE__,  __LINE__ );  \
                exit( EXIT_FAILURE ); }}


//__global__ cuda_function_test(){
//    cuprintf("call cuda function"); 
//}

#define N   10

__global__ void add( int *a,  int *b,  int *c ) {
    int tid = blockIdx.x;     // handle the data at this index
    if (tid < N)
        c[tid] = a[tid] + b[tid]; 
}

__global__ void dot(float* input1, float* input2,  float * output) {
    //@@ Load a segment of the input vector into shared memory
    __shared__ float cache[threadsPerBlock]; 
    int tid = threadIdx.x + blockIdx.x * blockDim.x; 
    int cacheIndex = threadIdx.x; 
    float temp = 0; 
    while(tid<ARRAY_LENGTH){
        temp  += input1[tid]*input2[tid]; 
        tid  += blockDim.x * gridDim.x; 
    }
    cache[cacheIndex] = temp; 
    __syncthreads(); 
    int i = blockDim.x/2; 
    while(i!=0){
        if(cacheIndex<i)
            cache[cacheIndex]  +=  cache[cacheIndex+1]; 
        __syncthreads(); 
        i  /=  2; 
    }
    if(cacheIndex  ==  0)
        output[blockIdx.x] = cache[0]; 

    return; 
}


__global__ void sumSquare(float * input,  float * output) {
    //@@ Load a segment of the input vector into shared memory
    __shared__ float cache[threadsPerBlock]; 
    int tid = threadIdx.x + blockIdx.x * blockDim.x; 
    int cacheIndex = threadIdx.x; 
    float temp = 0; 
    while(tid<ARRAY_LENGTH){
        temp  += input[tid]*input[tid]; 
        tid  += blockDim.x * gridDim.x; 
    }
    cache[cacheIndex] = temp; 
    __syncthreads(); 
    int i = blockDim.x/2; 
    while(i!=0){
        if(cacheIndex<i)
            cache[cacheIndex]  +=  cache[cacheIndex+1]; 
        __syncthreads(); 
        i  /=  2; 
    }
    if(cacheIndex  ==  0)
        output[blockIdx.x] = cache[0]; 

    return; 
}

__global__ void sum(float * input,  float * output) {
    //@@ Load a segment of the input vector into shared memory
    __shared__ float cache[threadsPerBlock]; 
    int tid = threadIdx.x + blockIdx.x * blockDim.x; 
    int cacheIndex = threadIdx.x; 
    float temp = 0; 
    while(tid<ARRAY_LENGTH){
        temp  += input[tid]; 
        tid  += blockDim.x * gridDim.x; 
    }
    cache[cacheIndex] = temp; 
    __syncthreads(); 
    int i = blockDim.x/2; 
    while(i!=0){
        if(cacheIndex<i)
            cache[cacheIndex]  +=  cache[cacheIndex+1]; 
        __syncthreads(); 
        i  /=  2; 
    }
    if(cacheIndex  ==  0)
        output[blockIdx.x] = cache[0]; 

    return; 
}

//fenzi = sum(X .* Y) - (sum(X) * sum(Y)) / length(X);   
//fenmu = sqrt((sum(X .^2) - sum(X)^2 / length(X)) * (sum(Y .^2) - sum(Y)^2 / length(X)));   
//coeff = fenzi / fenmu;   
extern "C" float  coefficient_single(float* a,  float* b){
    float* dev_a = a; 
    float* dev_b = b; 
 //   cudaMalloc(&dev_a,  sizeof(float) * ARRAY_LENGTH); 
 //   cudaMalloc(&dev_b,  sizeof(float) * ARRAY_LENGTH); 
 //   cudaMemcpy(dev_a,  a,  sizeof(float) * ARRAY_LENGTH,  cudaMemcpyHostToDevice); 
 //   cudaMemcpy(dev_b,  b,  sizeof(float) * ARRAY_LENGTH,  cudaMemcpyHostToDevice); 

    float* dev_o; 
    float* host_o = (float*) malloc(BLOCK_NUM* sizeof(float)); 
    cudaMalloc(&dev_o,  sizeof(float) * BLOCK_NUM); 
    // do following
    // dot x.*y
    dot <<<BLOCK_NUM, THREAD_NUM>>>(dev_a,  dev_b,  dev_o); 
    cudaMemcpy(host_o,  dev_o,  sizeof(float) * BLOCK_NUM,  cudaMemcpyDeviceToHost); 
    float dot = 0; 
    for (int ii = 0;  ii < BLOCK_NUM;  ii++) {
        dot +=  host_o[ii]; 
    }
    //cout  << "dot " << dot << endl; 
    
    // sum x
    sum<<<BLOCK_NUM, THREAD_NUM>>>(dev_a, dev_o); 
    cudaMemcpy(host_o,  dev_o,  sizeof(float) * BLOCK_NUM,  cudaMemcpyDeviceToHost); 
    float sum_x= 0; 
    for (int ii = 0;  ii < BLOCK_NUM;  ii++) {
        sum_x +=  host_o[ii]; 
    }
    //cout  << "sum_x" << sum_x << endl; 
    // sum y
    sum<<<BLOCK_NUM, THREAD_NUM>>>(dev_b, dev_o); 
    cudaMemcpy(host_o,  dev_o,  sizeof(float) * BLOCK_NUM,  cudaMemcpyDeviceToHost); 
    float sum_y= 0; 
    for (int ii = 0;  ii < BLOCK_NUM;  ii++) {
        sum_y +=  host_o[ii]; 
    }
    //cout  << "sum_y" << sum_y << endl; 

    // sum y.*2
    sum<<<BLOCK_NUM, THREAD_NUM>>>(dev_b, dev_o); 
    cudaMemcpy(host_o,  dev_o,  sizeof(float) * BLOCK_NUM,  cudaMemcpyDeviceToHost); 
    float sum_y_2= 0; 
    for (int ii = 0;  ii < BLOCK_NUM;  ii++) {
        sum_y_2 +=  host_o[ii]; 
    }
    //cout  << "sum_y_2" << sum_y_2 << endl; 

    // sum x.*2
    sum<<<BLOCK_NUM, THREAD_NUM>>>(dev_a, dev_o); 
    cudaMemcpy(host_o,  dev_o,  sizeof(float) * BLOCK_NUM,  cudaMemcpyDeviceToHost); 
    float sum_x_2= 0; 
    for (int ii = 0;  ii < BLOCK_NUM;  ii++) {
        sum_x_2 +=  host_o[ii]; 
    }
    //cout  << "sum_x_2" << sum_x_2 << endl; 

    //fenmu = sqrt((sum(X .^2) - sum(X)^2 / length(X)) * (sum(Y .^2) - sum(Y)^2 / length(X)));   
    float fenzi = (dot-sum_x*sum_y)/ARRAY_LENGTH; 
    float fenmu = sqrt((sum_x_2-sum_x*sum_x/ARRAY_LENGTH)*(sum_y_2-sum_y*sum_y/ARRAY_LENGTH)); 

//    cudaFree(dev_a); 
//    cudaFree(dev_b); 
    cudaFree(dev_o); 
    free(host_o); 
    return fenzi/fenmu; 
}


extern "C" void coefficient(stockPriceMap* data){
    int loop = SERVER_STOCK; 
    float* array[loop]; 
    for(int i=0;  i<loop;  ++i){
        float a_c[ARRAY_LENGTH]; 
        const deque<int>& q_a = (*data)[i]; 
        for(int c=0;  c<ARRAY_LENGTH; ++c){
            a_c[c]=q_a[c]; 
        }
        cudaMalloc(&array[i],  sizeof(float) * ARRAY_LENGTH); 
        cudaMemcpy(array[i],  a_c,  sizeof(float) * ARRAY_LENGTH,  cudaMemcpyHostToDevice); 
    }
    cout  << "start coefficient calculate" << endl; 
    float min_cof = 1; 
    int stock_x,  stock_y; 
    for(int i=0;  i<loop; ++i){
        for(int j=i+1; j<loop;  ++j){
            float val = coefficient_single(array[i],  array[j]); 
            val = fabs(val); 
            if((val)<min_cof){
                stock_x = i; 
                stock_y = j; 
                min_cof = (val); 
            }
        }
    }
    cout  << "Min coefficient factor:" << min_cof << " with stock:" << stock_x << " and stock:" << stock_y << endl; 


    for(int i=0;  i<loop;  ++i){
        cudaFree(array[loop]); 
    }
    abort(); 
    //stockPriceMap::const_iterator s = data->begin(),  e = data->end(); 
    //for(; s!=e; ++s){
    //    cout  <<"stock id:" <<  s->first << " " << s->second.size() << endl; 
    //}
}


extern "C" void cuda_test(){
   /// int deviceCount; 
   /// CUDA_SAFE_CALL(cudaGetDeviceCount(&deviceCount)); 
   /// if(deviceCount  == 0)
   ///     printf("There is no device support CUDA(GPU)"); 
   /// int dev; 
   /// for(dev=0;  dev<deviceCount; ++dev){
   /// 
   /// }

    int a[N],  b[N],  c[N]; 
    int *dev_a,  *dev_b,  *dev_c; 
    // allocate the memory on the GPU
    HANDLE_ERROR( cudaMalloc( (void**)&dev_a,  N * sizeof(int) ) ); 
    HANDLE_ERROR( cudaMalloc( (void**)&dev_b,  N * sizeof(int) ) ); 
    HANDLE_ERROR( cudaMalloc( (void**)&dev_c,  N * sizeof(int) ) ); 
    // fill the arrays 'a' and 'b' on the CPU
    for (int i=0;  i<N;  i++) {
        a[i] = -i; 
        b[i] = i * i; 
    }

    // copy the arrays 'a' and 'b' to the GPU
    HANDLE_ERROR( cudaMemcpy( dev_a,  a,  N * sizeof(int), 
                cudaMemcpyHostToDevice ) ); 
    HANDLE_ERROR( cudaMemcpy( dev_b,  b,  N * sizeof(int), 
                cudaMemcpyHostToDevice ) ); 
    add << <N, 1 >> >( dev_a,  dev_b,  dev_c ); 
    // copy the array 'c' back from the GPU to the CPU
    HANDLE_ERROR( cudaMemcpy( c,  dev_c,  N * sizeof(int), 
                cudaMemcpyDeviceToHost ) ); 
    // display the results
    for (int i=0;  i<N;  i++) {
        printf( "%d + %d = %d\n",  a[i],  b[i],  c[i] ); 
    }
    // free the memory allocated on the GPU
    cudaFree( dev_a ); 
    cudaFree( dev_b ); 
    cudaFree( dev_c ); 
}
