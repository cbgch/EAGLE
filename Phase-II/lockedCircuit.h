#ifndef LOCKEDCIRCUIT_H
#define LOCKEDCIRCUIT_H

#include<bits/stdc++.h>
#include "graph.h"
using namespace std;
class LockedCircuit{
public:
    string lockFun;
    string circuit;
    vector<int> nodes;
    int nodeNums=0;

    Graph g;
    vector<int> inNum;
    vector<int> function;
    vector<vector<int>> inputPort;
    
    set<int> keyGates;         
    set<int> inputGates;      
    set<int> outputGates;      
    map<int,int> realKey;    
    int keySize=0;

    set<string> inputPortName;     
    set<string> keyPortName;       
    set<string> outputPortName;    

    map<int,vector<int>> in2Gate; 
    map<int,int> out2Gate; 

    map<int,int> gateId2Vec;

    map<string,int> outgateOrder;  
    vector<string> orderOutGate;   
    map<string,int> ingateOrder;  
    vector<string> orderInGate;   
    // map<string,int> keygateOrder;  
    // vector<string> orderKeyGate; 
public:
    LockedCircuit(string lock,string circuit);
    void initCircuit();
    string keySim(string ,string );
};

#endif