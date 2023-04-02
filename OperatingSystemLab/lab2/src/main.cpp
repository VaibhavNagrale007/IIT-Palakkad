#include<bits/stdc++.h>
#include <unistd.h>
#include <stdio.h>
#include "../include/Logger.h"
#include "../include/eval.h"

using namespace std;
 
typedef long long ll;

int main(int argc, char **argv){
    // storing parsed arguments
    CLogger::GetLogger()->Log("=========================================================================\n\nLogging Started.");
    vector<char> args;
    ll option;

    //parse arguments.
    while ((option = getopt(argc, argv, ":asmdASMTX")) != -1) {
        switch (option) {
            case 'a':
            case 's':
            case 'm':
            case 'd':
            case 'A':
            case 'S':
            case 'M':
            case 'T':
            case 'X':
                args.emplace_back(option);  // add it to array/vector
                break;
            // if it is an invalid/unknown argument
            default:
                {
                    CLogger::GetLogger()->Log("Invalid Arguments given!");
                    exit(0);
                }
        }
    }
    //Logging execution status.
    CLogger::GetLogger()->Log("Arguments successfully parsed.");
	
    vector<ll> scalars;
    vector<vector<vector<ll>>> matrices;
    string order="";

    string line="";

    //checking for validity of given input.
    while (getline(cin, line)){
        // for empty line and comment
        if (line.length() == 0 ) continue;
        bool cmtflg = false;
        for (auto cmt: line) if (cmt == '#') cmtflg=true;
        if (cmtflg == true) continue;

        // validity checker [ digit | space | comma | - ]
        bool chkflg=false;
        for (auto chk: line) if (!isdigit(chk) && chk != ' ' && chk != ',' && chk != '-') 
            chkflg = true;
        if (chkflg){
            CLogger::GetLogger()->Log("Given input file is not in valid format, Parsing Terminated.");
            exit(0);
        }
        // cout << line << endl;
        
    	//checking for number of inputs.    
        int state = 0, nc = 0;
        for(auto str: line){
            if (str == ' ' || str == '\n' || str == '\t'){
                state = 0;
            }
            else if (state == 0)
            {
                state = 1;
                ++nc;
            }
        }
        if (nc==1) {
		//if input is 1, then it is  scalar.
            scalars.emplace_back(stoll(line));
        }
        else{   // nc==2
		//else matrix input.
		//for parsing the dimensions of matrix.
            ll row=0, column=0;
            state=0;
            string s="";
            for(auto str: line){
                if (str == ' ' || str == '\n' || str == '\t'){
                    state = 0;
                    if (row==0) row = stoll(s);
                    else column = stoll(s);
                    s="";
                }
                else if (state == 0)
                {
                    state = 1;
                    s+=str;
                }
            }

	    //parsing matrix into vector.
            if (column==0) column=stoll(s);
            // cout << row << " " << column << endl;
            vector<vector<ll>> ts;
            for (int i=0; i<row; i++){
                getline(cin, line);
                state=0;s="";
                vector<ll> tss;
                for(auto str: line){
                    if (str == ','){
                        state = 0;
                        tss.emplace_back(stoll(s));
                        s="";
                    }
                    else if (state == 0)
                    {
                        state = 1;
                        s+=str;
                    }
                }
                ts.emplace_back(tss);
            }
            matrices.emplace_back(ts);
        }
    }
    CLogger::GetLogger()->Log("Input file successfully Parsed.");

    
    // printing scalars and vectors (for debug)
    // for (int i=scalars.size()-1; i>=0; i--){
    //     // cout << scalars[i] <<" ";
    // }
    // cout << endl;
    // for (int i=0; i<matrices.size(); i++){
    //     for (int j=0; j<matrices[i].size(); j++){
    //         for (int k=0; k<matrices[i][j].size(); k++){
    //             // cout << matrices[i][j][k] <<" ";
    //         }
    //         // cout << endl;
    //     }
    //     // cout <<"============="<< endl;
    // }

    //stack for arithmetic operations.
    stack<ll> ss;
    stack<vector<vector<ll>>> sm;
    for (int i=scalars.size()-1; i>=0; i--){
        ss.push(scalars[i]);
    }
    for (int i=matrices.size()-1; i>=0; i--){
        sm.push(matrices[i]);
    }

    // perform operations
    // eval::add(scalars[0], matrices[0]); 
    // corresponding to arguments, evaluate.
    vector<vector<ll>> m0, m1;
    ll s0;
    char flag='z';
    for (auto a: args){
        switch (a)
        {
            case 'a':{  if(sm.size() > 0 && ss.size() > 0){
                            s0=ss.top();ss.pop();
                            m0=sm.top();sm.pop();
                            eval::add(s0, m0);
                            sm.push(m0);
                            flag=a;
                        }
                        else  {
                            CLogger::GetLogger()->Log("Insufficient Matrix or Scalar [scalar addition]."); 
                            exit(0);
                        }
                        break;
                    }
            case 's':{  if(sm.size() > 0 && ss.size() > 0){
                            s0=ss.top();ss.pop();
                            m0=sm.top();sm.pop();
                            eval::subs(s0, m0);
                            sm.push(m0);
                            flag='s';
                        }
                        else {
                            CLogger::GetLogger()->Log("Insufficient Matrix or Scalar [scalar subtraction]."); 
                            exit(0);
                        }
                        break;
                    }
            case 'm':{  if(sm.size() > 0 && ss.size() > 0){
                            s0=ss.top();ss.pop();
                            m0=sm.top();sm.pop();
                            eval::muls(s0, m0);
                            sm.push(m0);
                            flag='m';
                        }
                        else {
                            CLogger::GetLogger()->Log("Insufficient Matrix or Scalar.[scalar multiplication]."); 
                            exit(0);
                        }
                        break;
                    }
            case 'd':{  if(sm.size() > 0 && ss.size() > 0){
                            s0=ss.top();ss.pop();
                            m0=sm.top();sm.pop();
                            eval::divs(s0, m0);
                            sm.push(m0);
                            flag='d';
                        }
                        else {
                            CLogger::GetLogger()->Log("Insufficient Matrix or Scalar.[scalar division]"); 
                            exit(0);
                        }
                        break;
                    }
            case 'A':{  if(sm.size() > 1){
                            m0=sm.top();sm.pop();
                            m1=sm.top();sm.pop();
                            eval::addm(m0, m1);
                            sm.push(m0);
                            flag='A';
                        }
                        else {
                            CLogger::GetLogger()->Log("Insufficient Matrix or Scalar.[Matrix addition]");
                            exit(0);
                        }
                        break;
                    }
            case 'S':{  if(sm.size() > 1){
                            m0=sm.top();sm.pop();
                            m1=sm.top();sm.pop();
                            eval::subm(m0, m1);
                            sm.push(m0);
                            flag='S';
                        }
                        else {
                            CLogger::GetLogger()->Log("Insufficient Matrix or Scalar.[Matrix Subtraction]");
                            exit(0);
                        }
                        break;
                    }
            case 'M':{  if(sm.size() > 1){
                            m0=sm.top();sm.pop();
                            m1=sm.top();sm.pop();
                            eval::mulm(m0, m1);
                            sm.push(m0);
                            flag='M';
                        }
                        else {
                            CLogger::GetLogger()->Log("Insufficient Matrix or Scalar.[Matrix Multiplication]");
                            exit(0);
                        }
                        break;
                    }
            case 'T':{  if(sm.size() > 0){
                            m0=sm.top();sm.pop();
                            eval::transm(m0);
                            sm.push(m0);
                            flag='T';
                        }
                        else {
                            CLogger::GetLogger()->Log("Insufficient Matrix or Scalar.[Transpose]");
                            exit(0);
                        }
                        break;
                    }
            case 'X':{  if(sm.size() > 0){
                            m0=sm.top();sm.pop();
                            ll detval=INT_MIN;
                            eval::detm(m0, detval);
                            if (detval!=INT_MIN) ss.push(detval);
                            flag='X';
                        }
                        else {
                            CLogger::GetLogger()->Log("Insufficient Matrix or Scalar.[Determinant]");
                            exit(0);
                        }
                        break;
                    }
            default: {
                    CLogger::GetLogger()->Log("Encountered Invalid Operator.");
                    exit(0);
                }
                    break;
        }
    }

    
    // if the last performed operation is determinant, then output the top of scalar stack, else the top of matrix stack.
    if (flag=='X') cout << ss.top() << endl;
    else if (flag!='z') {
        m0=sm.top();
        cout << m0.size() << " " << m0[0].size() << endl;
        for (int i=0; i<m0.size(); i++){
            for (int j=0; j<m0[0].size(); j++){
                cout << m0[i][j] <<",";
            }
            cout << endl;
        }
    }
    return EXIT_SUCCESS;
}
