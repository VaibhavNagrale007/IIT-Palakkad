#include "../include/eval.h"
#include "../include/Logger.h"

#include<vector>
#include<iostream>

typedef long long ll;

void eval::muls(ll &scaler, std::vector<std::vector<long long>> &matrix){
    for (int i=0; i<matrix.size(); i++){
        for (int j=0; j<matrix[i].size(); j++){
            matrix[i][j]*=scaler;
        }
    }
    CLogger::GetLogger()->Log("Successful. [Scalar Multiplication]");
}

