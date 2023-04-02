#include "../include/eval.h"
#include "../include/Logger.h"

#include<vector>
#include<iostream>

typedef long long ll;

void eval::divs(ll &scaler, std::vector<std::vector<long long>> &matrix){
	if(scaler != 0){
    		for (int i=0; i<matrix.size(); i++){
        		for (int j=0; j<matrix[i].size(); j++){
            			matrix[i][j]/=scaler;
        		}
    		}

			CLogger::GetLogger()->Log("Scalar Division successful.");
	}
	else{
		//todo handle error.
		std::cout << "div by 0 not allowed" << std::endl;
		CLogger::GetLogger()->Log("Div by 0 not allowed.[Scalar Division]");
		exit(0);
	}
}

