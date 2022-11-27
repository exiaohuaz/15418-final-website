typedef std::pair<int,int> P;
P indices[9] = {P(-1, -1), P(-1, 0), P(-1, 1), P(0, -1),  P(0, 0),  P(0, 1), P(1, -1),  P(1, 0),  P(1, 1)};
int mask[3][3] = {
                {-1, -1, -1},
                {-1, 8, -1},
                {-1, -1, -1}
                };

export void convolve_ispc(uniform int rows, 
                          uniform int cols,
                          uniform int values[][],
                          uniform int output[][])
{
    
    foreach(i = 1 ... rows, j = 1 ... cols){
        

    }
}