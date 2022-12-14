//not sure if this is valid lol
int indices[9][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
int mask[3][3] = {
                {-1, -1, -1},
                {-1, 8, -1},
                {-1, -1, -1}
                };

//not sure what types to make values and output
export void convolution_ispc(uniform int rows, 
                          uniform int cols,
                          uniform int values[],
                          uniform int output[])
{
    foreach(i = 1 ... rows-1, j = 1 ... cols-1){
        int index = rows*i + j;
        int sum = 0;
        for(int loc = 0; loc < 9; loc++) {
            int compute_index = rows*(i+indices[loc][0]) + (j+indices[loc][1]);
            sum += values[compute_index] * mask[1+indices[loc][0]][1+indices[loc][1]];
        }
        output[index] = sum<=120?255:0;
    }
}


/*
export void convolution_ispc_withtasks(uniform int rows, 
                          uniform int cols,
                          int values[][],
                          int output[][])
{
    uniform int threadCount = 8;
    uniform int rowsPerTask = (rows / threadCount);
    launch[threadCount] convolution_ispc_task(rowsPerTask, cols, values, output);
}*/


