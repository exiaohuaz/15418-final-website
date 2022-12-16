//#include <png.h>
//#include "/usr/local/include/png.h"
#include <stdio.h>
#include <stdlib.h>
//#include <zlib.h>
#include <assert.h>
//#include "/usr/local/include/png++/png.hpp"
//#include "/usr/local/opt/libomp/include/omp.h"
#include <png++/png.hpp>
#include "timing.h"
#include "CycleTimer.h"
#include "convolution_ispc.h"
#include <omp.h>

typedef std::pair<int,int> P;
P indices[9] = {P(-1, -1), P(-1, 0), P(-1, 1), P(0, -1),  P(0, 0),  P(0, 1), P(1, -1),  P(1, 0),  P(1, 1)};
int mask[3][3] = {
                {-1, -1, -1},
                {-1, 8, -1},
                {-1, -1, -1}
                };

void verify(int rows, int cols, png::image<png::gray_pixel> output, png::image<png::gray_pixel> output_parallel){
    for(int i = 0; i<rows; i++){
        for(int j = 0; j<rows; j++){
            assert(output[i][j] == output_parallel[i][j]);
        }
    }
}

void convolution_sequential(int rows, int cols, png::image<png::gray_pixel> &img, png::image<png::gray_pixel> &output){
    for(int i = 1; i<rows-1; i++){ //edges arent computable
        for(int j = 1; j<cols-1; j++){
            int sum = 0;
            for(int loc = 0; loc < sizeof(indices)/sizeof(P); loc++){
                P diff = indices[loc];
                sum += img[i+diff.first][j+diff.second] * mask[1+diff.first][1+diff.second];
            }
            output[i][j] = sum<=120?255:0;
        }
    }
}

void convolution_openmp(int rows, int cols, png::image<png::gray_pixel> &img, png::image<png::gray_pixel> &output){
    #pragma omp parallel for collapse(2)
    for(int i = 1; i<rows-1; i++){ //edges arent computable
        for(int j = 1; j<cols-1; j++){
            int sum = 0;
            for(int loc = 0; loc < sizeof(indices)/sizeof(P); loc++){
                P diff = indices[loc];
                sum += img[i+diff.first][j+diff.second] * mask[1+diff.first][1+diff.second];
            }
            output[i][j] = sum<=120?255:0;
        }
    }
}

int main(int argc, char** argv) {
    png::image<png::gray_pixel> img(argv[1]);
    int rows = img.get_height();
    int cols = img.get_width();
    printf("rows: %d  cols: %d\n", rows, cols);
    png::image<png::gray_pixel> output(cols, rows);

    if (argc > 3 && strcmp(argv[2], "--profile") == 0) {
        // profile code
        printf("Profiling code...\n");
        const int iterations = 3;
        double totalSeqTime = 0;
        double totalParTime = 0;
        for (int i = 0; i < iterations + 1; i++) {
            double startTime = CycleTimer::currentSeconds();
            convolution_sequential(rows, cols, img, output);
            double endTime = CycleTimer::currentSeconds();

            if (i == 0) continue; // rid locality benefit

            double delTime = endTime - startTime;
            totalSeqTime += delTime;
            printf("total sequential time trial %d: %.6fs\n", i - 1, delTime);
        }

        int numThreads = atoi(argv[3]);
        omp_set_num_threads(numThreads);

        png::image<png::gray_pixel> output_openmp(cols, rows);
        for (int i = 0; i < iterations + 1; i++) {
            double startTime = CycleTimer::currentSeconds();
            convolution_openmp(rows, cols, img, output_openmp);
            double endTime = CycleTimer::currentSeconds();

            if (i == 0) continue; // rid locality benefit
            
            double delTime = endTime - startTime;
            totalParTime += delTime;
            printf("total OpenMP time trial %d: %.6fs\n", i, delTime);
        }

        double avgSeq = totalSeqTime / iterations;
        double avgPar = totalParTime / iterations;
        double speedup = avgSeq / avgPar;
        printf("Total Speedup for Convolution on %d threads: %f\n", numThreads, speedup);
        return 0;
    }
    double startTime = CycleTimer::currentSeconds();
    convolution_sequential(rows, cols, img, output);
    double endTime = CycleTimer::currentSeconds();
    printf("total sequential simulation time: %.6fs\n", endTime - startTime);
    output.write("../images/outputConv.png");

    
    png::image<png::gray_pixel> output_ispc(cols, rows);
    int inputarr[cols*rows];
    for(int i = 0; i<rows; i++){
        for(int j = 0; j<cols; j++){
            inputarr[cols*i + j] = img[i][j];
        }
    }
    int outputarr[cols*rows];
    startTime = CycleTimer::currentSeconds();
    ispc::convolution_ispc(rows, cols, inputarr, outputarr);
    endTime = CycleTimer::currentSeconds();
    for(int i = 0; i<rows; i++){
        for(int j = 0; j<cols; j++){
            output_ispc[i][j] = outputarr[cols*i + j];
        }
    }
    output_ispc.write("../images/new.png");
    verify(rows, cols, output, output_ispc);
    printf("total ispc simulation time: %.6fs\n", endTime - startTime);
    

    png::image<png::gray_pixel> output_openmp(cols, rows);
    startTime = CycleTimer::currentSeconds();
    convolution_openmp(rows, cols, img, output_openmp);
    endTime = CycleTimer::currentSeconds();
    verify(rows, cols, output, output_openmp);
    printf("total openmp simulation time: %.6fs\n", endTime - startTime);
    printf("openmp and sequential outputs match!\n");
}
