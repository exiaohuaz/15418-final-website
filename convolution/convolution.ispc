//not sure if this is valid lol
typedef std::pair<int,int> P;
P indices[9] = {P(-1, -1), P(-1, 0), P(-1, 1), P(0, -1),  P(0, 0),  P(0, 1), P(1, -1),  P(1, 0),  P(1, 1)};
int mask[3][3] = {
                {-1, -1, -1},
                {-1, 8, -1},
                {-1, -1, -1}
                };

//not sure what types to make values and output
export void convolution_ispc(uniform int rows, 
                          uniform int cols,
                          int values[][],
                          int output[][])
{
    foreach(i = 1 ... rows-1, j = 1 ... cols-1){
        output[i][j] = 0;
    }
    foreach(i = 1 ... rows-1, j = 1 ... cols-1, loc = 0 ... (sizeof(indices)/sizeof(P))){
        P diff = indices[loc];
        output[i][j] += img[i+diff.first][j+diff.second] * mask[1+diff.first][1+diff.second];
    }
    foreach(i = 1 ... rows-1, j = 1 ... cols-1){
        output[i][j] = sum<=120?255:0
    }
}


//not complete
export void convolution_ispc_withtasks(uniform int rows, 
                          uniform int cols,
                          int values[][],
                          int output[][])
{
    uniform int threadCount = 8;
    uniform int rowsPerTask = (rows / threadCount);
    launch[threadCount] convolution_ispc_task(rowsPerTask, cols, values, output);
}