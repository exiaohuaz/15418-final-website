#include <png.h>
#include "/usr/local/include/png.h"
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <assert.h>
#include "/usr/local/include/png++/png.hpp"

#include "timing.h"
#include "CycleTimer.h"
#include "convolution_ispc.h"

typedef std::pair<int,int> P;
P indices[9] = {P(-1, -1), P(-1, 0), P(-1, 1), P(0, -1),  P(0, 0),  P(0, 1), P(1, -1),  P(1, 0),  P(1, 1)};
int mask[3][3] = {
                {-1, -1, -1},
                {-1, 8, -1},
                {-1, -1, -1}
                };

void verify(png::image<png::gray_pixel> output, png::image<png::gray_pixel> output_parallel){
    for(int i = 0; i<rows; i++){
        for(int j = 0; j<rows; j++){
            assert(output[i][j] == output_parallel[i][j]);
        }
    }
}

int main(int argc, char** argv) {
    png::image<png::gray_pixel> img(argv[1]);
    int rows = img.get_height();
    int cols = img.get_width();
    printf("rows: %d  cols: %d\n", rows, cols);
    png::image<png::gray_pixel> output(cols, rows);


    Timer totalSimulationTimer;

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

    double totalSimulationTime = totalSimulationTimer.elapsed();
    printf("total sequential simulation time: %.6fs\n", totalSimulationTime);
    output.write("out/output.png");

    
    //this stuff is new and doesn't work
    png::image<png::gray_pixel> output_ispc(cols, rows);
    double startTime = CycleTimer::currentSeconds();
    ispc::convolution_ispc(rows, cols, img, output_ispc);
    double endTime = CycleTimer::currentSeconds();
    verify(output, output_ispc);
    printf("total ispc simulation time: %.6fs\n", endTime - startTime);
}