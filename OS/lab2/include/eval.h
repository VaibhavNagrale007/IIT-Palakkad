#ifndef _EVAL_H_
#define _EVAL_H_
#include <vector>
#include<bits/stdc++.h>

namespace eval{
    void add(long long &scaler, std::vector<std::vector<long long>> &matrix);
    void subs(long long &scaler, std::vector<std::vector<long long>> &matrix);
    void muls(long long &scaler, std::vector<std::vector<long long>> &matrix);
    void divs(long long &scaler, std::vector<std::vector<long long>> &matrix);
    void addm(std::vector<std::vector<long long>> &matrix1, std::vector<std::vector<long long>> &matrix2);
    void subm(std::vector<std::vector<long long>> &matrix1, std::vector<std::vector<long long>> &matrix2);
    void mulm(std::vector<std::vector<long long>> &matrix1, std::vector<std::vector<long long>> &matrix2);
    void transm(std::vector<std::vector<long long>> &matrix1);
    void detm(std::vector<std::vector<long long>> &matrix1, long long &detval); 
}

#endif  // _EVAL_H_
