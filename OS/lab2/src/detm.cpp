#include <bits/stdc++.h>
#include "../include/eval.h"
#include "../include/Logger.h"
using namespace std;

using matrix = vector< vector<long long> >;
void eval::detm(matrix &mat, long long &detval)
{	
	int n = mat.size();
	bool isValid = true;
	for(int i=0; i<n; i++){
		if(mat[i].size() != n){
			isValid = false;
			break;
		}
	}

	if(isValid){
		int num1, num2, det = 1, index,
						total = 1; // Initialize result

		// temporary array for storing row
		int temp[n + 1];

		// loop for traversing the diagonal elements
		for (int i = 0; i < n; i++)
		{
			index = i; // initialize the index

			// finding the index which has non zero value
			while (index < n && mat[index][i] == 0)
			{
				index++;
			}
			if (index == n) // if there is non zero element
			{
				// the determinant of matrix as zero
				continue;
			}
			if (index != i)
			{
				// loop for swapping the diagonal element row and
				// index row
				for (int j = 0; j < n; j++)
				{
					swap(mat[index][j], mat[i][j]);
				}
				// determinant sign changes when we shift rows
				// go through determinant properties
				det = det * pow(-1, index - i);
			}

			// storing the values of diagonal row elements
			for (int j = 0; j < n; j++)
			{
				temp[j] = mat[i][j];
			}
			// traversing every row below the diagonal element
			for (int j = i + 1; j < n; j++)
			{
				num1 = temp[i]; // value of diagonal element
				num2 = mat[j][i]; // value of next row element

				// traversing every column of row
				// and multiplying to every row
				for (int k = 0; k < n; k++)
				{
					// multiplying to make the diagonal
					// element and next row element equal
					mat[j][k]
						= (num1 * mat[j][k]) - (num2 * temp[k]);
				}
				total = total * num1; // Det(kA)=kDet(A);
			}
		}

		// multiplying the diagonal elements to get determinant
		for (int i = 0; i < n; i++)
		{
			det = det * mat[i][i];
		}

		// cout << det / total << endl;
		detval = (long long)(det / total);
		LOGGER->Log("Determinant : %d", detval);
	}
	else{
		// TODO: handle error
		CLogger::GetLogger()->Log("Invalid Matrix Dimention for calculating Determinant.");
		exit(0);
	}
}