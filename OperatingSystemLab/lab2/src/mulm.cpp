#include "../include/eval.h"
#include "../include/Logger.h"

#include<vector>
#include<iostream>

typedef long long ll;

void eval::mulm(std::vector<std::vector<long long>> &matrix1, std::vector<std::vector<long long>> &matrix2){
    if (matrix1[0].size()==matrix2.size()){
        std::vector<std::vector<ll>> rslt(matrix1.size(), std::vector<ll> (matrix2[0].size(), 0));
        for (int i = 0; i < matrix1.size(); i++) {
            for (int j = 0; j < matrix2[0].size(); j++) {
                rslt[i][j] = 0;
                for (int k = 0; k < matrix2.size(); k++) {
                    rslt[i][j] += matrix1[i][k] * matrix2[k][j];
                }
                // std::cout << rslt[i][j] <<" ";
            }
            // std::cout << std::endl;
        }
        matrix1=rslt;
        CLogger::GetLogger()->Log("Successful.[Matrix Multiplication]");
    }else{
        // TODO: handle error
        CLogger::GetLogger()->Log("Size mismatch.[Matrix Multiplication]");
        exit(0);
    }
    
}

