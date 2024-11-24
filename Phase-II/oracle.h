#ifndef ORACLE_H
#define ORACLE_H

#include <bits/stdc++.h>
#include "graph.h"
using namespace std;
class Oracle{
public:
    string lockFun="";
    string circuit="";
    vector<int> nodes; 
    int nodeNums=0;

    vector<int> inNum;  
    vector<int> function;  
    vector<vector<int>> inputPort;

    Graph g;

    set<int> inputGates;       
    set<int> outputGates;     

    set<string> inputPortName;     
    set<string> outputPortName;    

    map<int,vector<int>> in2Gate;  
    map<int,int> out2Gate;  

    map<string,int> outgateOrder;  
    vector<string> orderOutGate;   
    map<string,int> ingateOrder;   
    vector<string> orderInGate;    
public:
    Oracle(string lock,string circuit);
    void initOracle();
    string oracleSim(string);
};

#endif