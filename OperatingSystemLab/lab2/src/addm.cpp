#include "../include/eval.h"
#include "../include/Logger.h"

#include<vector>
#include<iostream>

typedef long long ll;

void eval::addm(std::vector<std::vector<long long>> &matrix1, std::vector<std::vector<long long>> &matrix2){
    if (matrix1.size()==matrix2.size() && matrix1[0].size()==matrix2[0].size()){
        for (int i=0; i<matrix1.size(); i++){
            for (int j=0; j<matrix1[i].size(); j++){
                matrix1[i][j]+=matrix2[i][j];
            }
        }
        CLogger::GetLogger()->Log("Matrix addition done successfully.");
    }else{
        // TODO: handle error
        CLogger::GetLogger()->Log("Matrix dimention mismatched for Addition.");
        exit(0);
    }
    
}
