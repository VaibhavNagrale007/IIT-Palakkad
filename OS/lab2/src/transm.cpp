#include "../include/eval.h"
#include "../include/Logger.h"

#include<vector>
#include<iostream>

typedef long long ll;
    
void eval::transm(std::vector<std::vector<long long>> &matrix1){
	std::vector<std::vector <long long>> temp( matrix1[0].size(), std::vector<long long> (matrix1.size(), 0));

	for(int i=0; i<matrix1.size(); i++){
		for(int j=0; j<matrix1[i].size(); j++){
			temp[j][i] = matrix1[i][j];
		}
	}
    matrix1 = temp;

	CLogger::GetLogger()->Log("Successful. [Matrix Transpose]");
}