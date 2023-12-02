#include <stdio.h>
#include <algorithm>
#include "CycleTimer.h"
#include "mkl.h"

void saxpySerial(int N,
                       float scale,
                       float X[],
                       float Y[],
                       float result[])
{

    for (int i=0; i<N; i++) {
        result[i] = scale * X[i] + Y[i];
    }
}

// return GB/s
static float
toBW(int bytes, float sec) {
    return static_cast<float>(bytes) / (1024. * 1024. * 1024.) / sec;
}

static float
toGFLOPS(int ops, float sec) {
    return static_cast<float>(ops) / 1e9 / sec;
}

static void verifyResult(int N, float* result, float* gold) {
    for (int i=0; i<N; i++) {
        if (result[i] != gold[i]) {
            printf("Error: [%d] Got %f expected %f\n", i, result[i], gold[i]);
        }
    }
}

int main()
{
    mkl_set_num_threads(32);

	const unsigned int N = 20 * 1000 * 1000; // 20 M element vectors (~80 MB)
    const unsigned int TOTAL_BYTES = 4 * N * sizeof(float);
    const unsigned int TOTAL_FLOPS = 2 * N;
    float scale = 2.f;

    float* arrayX = new float[N];
    float* arrayY = new float[N];
    float* resultSerial = new float[N];
    float* resultMKL = new float[N]; // MKL result array

    // initialize array values
    for (unsigned int i = 0; i < N; i++) {
        arrayX[i] = i;
        arrayY[i] = i;
        resultSerial[i] = 0.f;
        resultMKL[i] = 0.f;
    }

    //
    // Run the serial implementation. Repeat three times for robust
    // timing.
    //
    double minSerial = 1e30;
    for (int i = 0; i < 3; ++i) {
        double startTime =CycleTimer::currentSeconds();
        saxpySerial(N, scale, arrayX, arrayY, resultSerial);
        double endTime = CycleTimer::currentSeconds();
        minSerial = std::min(minSerial, endTime - startTime);
    }

    printf("[saxpy serial]:\t\t[%.3f] ms\t[%.3f] GB/s\t[%.3f] GFLOPS\n",
        minSerial * 1000,
        toBW(TOTAL_BYTES, minSerial),
        toGFLOPS(TOTAL_FLOPS, minSerial));

    //
    // MKL saxpy implementation
    //
    double minMKL = 1e30;
    for (int i = 0; i < 3; ++i) {
        std::copy(arrayY, arrayY + N, resultMKL); // Reset resultMKL to initial values
        double startTime = CycleTimer::currentSeconds();
        cblas_saxpy(N, scale, arrayX, 1, resultMKL, 1); // MKL saxpy call
        double endTime = CycleTimer::currentSeconds();
        minMKL = std::min(minMKL, endTime - startTime);
    }

    verifyResult(N, resultMKL, resultSerial);

    printf("[saxpy MKL]:\t\t[%.3f] ms\t[%.3f] GB/s\t[%.3f] GFLOPS\n",
           minMKL * 1000,
           toBW(TOTAL_BYTES, minMKL),
           toGFLOPS(TOTAL_FLOPS, minMKL));

    // ... [rest of your code]

    delete[] arrayX;
    delete[] arrayY;
    delete[] resultSerial;
    delete[] resultMKL; // Don't forget to delete the MKL result array

	return 0;
}
