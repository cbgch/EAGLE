#include<bits/stdc++.h>
#include "lockedCircuit.h"
#include "util.h"
using namespace std;

LockedCircuit::LockedCircuit(string lock,string circuit){
        this->lockFun=lock;
        this->circuit=circuit;
        fstream countFile;
        countFile.open(lockFun+"/"+circuit+"/lockedCircuit/count.txt",ios::in);
        string line="";
        while(getline(countFile,line)){
            trim(line);
            // cout<<line<<endl;
            int val = atoi(line.c_str());
            nodes.push_back(val);
            nodeNums=val>nodeNums?val:nodeNums;
        }
        ++nodeNums;
        countFile.close();
        this->g=Graph(nodeNums);
        inNum.resize(nodeNums);
        function.resize(nodeNums);
        inputPort.resize(nodeNums,vector<int>(0));
    }
void LockedCircuit::initCircuit(){
    string line;
    fstream posKey,negKey;
    negKey.open(lockFun+"/"+circuit+"/lockedCircuit/node_neg.txt",ios::in); 
    posKey.open(lockFun+"/"+circuit+"/lockedCircuit/node_pos.txt",ios::in); 
    while(getline(posKey,line)){
        trim(line);
        // cout<<line<<endl;
        int val = atoi(line.c_str());
        realKey[val]=0;
    }
    while(getline(negKey,line)){
        trim(line);
        // cout<<line<<endl;
        int val = atoi(line.c_str());
        realKey[val]=1;
    }
    negKey.close();
    posKey.close();

    fstream linkFile;
    linkFile.open(lockFun+"/"+circuit+"/lockedCircuit/link.txt",ios::in);
    while(getline(linkFile,line)){
        trim(line);
        // cout<<line<<endl;
        vector<string> ret= split(line," ");
        trim(ret[0]);trim(ret[1]);
        int source = atoi(ret[0].c_str());
        int target = atoi(ret[1].c_str());
        if(source!=target){
            g.addEdge(source,target);
        }else{
            cout<<"self connection: "<<source<<"  "<<target<<endl;
        }
    }
    linkFile.close();

    fstream featFile;
    featFile.open(lockFun+"/"+circuit+"/lockedCircuit/feat.txt",ios::in);
    
    int curPosId=0;
    while(getline(featFile,line)){
        trim(line);
        // cout<<line<<endl;
        vector<string> ret= split(line," ");
        int curGateId=nodes[curPosId++]; 
        for(int x=0;x<ret.size();x++){
            trim(ret[x]);
            int val=atoi(ret[x].c_str());
            //3:xor  4:xnor  5:and  6:or  7:nand  8:nor  9:inv  10:buf
            if(x>=3&&val==1){
                function[curGateId]=x;
                // if(x==9||x==10){
                //     inNum[curGateId]=1;
                // }else{
                //     inNum[curGateId]=2;
                // }
            }
        }
    }
    featFile.close();

    fstream netInFile,netOutFile;
    netInFile.open(lockFun+"/"+circuit+"/lockedCircuit/Netlist_Inputs.txt",ios::in);
    netOutFile.open(lockFun+"/"+circuit+"/lockedCircuit/Netlist_Outputs.txt",ios::in);

    curPosId=0;
    while(getline(netOutFile,line)){
        trim(line);
        vector<string> ret= split(line," ");
        string portName=ret[0];
        trim(portName);

        outputPortName.insert(portName);

        outgateOrder[portName]=curPosId;  
        orderOutGate.push_back(portName);  
        ++curPosId;
    }
    curPosId=0;
    while(getline(netInFile,line)){
        trim(line);
        vector<string> ret= split(line," ");
        string portName=ret[0];
        trim(portName);
        if(portName[0]=='K'||portName[0]=='k'){
            keyPortName.insert(portName);
            ++keySize;
        }else{
            inputPortName.insert(portName);
        }
        ingateOrder[portName]=curPosId;   
        orderInGate.push_back(portName);   
        ++curPosId;
    }
    netInFile.close();
    netOutFile.close();

    fstream LogInfoFile;
    LogInfoFile.open(lockFun+"/"+circuit+"/lockedCircuit/logInfo.txt",ios::in);

    while(getline(LogInfoFile,line)){
        trim(line);
        vector<string> ret= split(line," ");
        int lens=ret.size();


        trim(ret[lens-1]);
        int gateId=atoi(ret[lens-1].c_str());

        string outPort=ret[lens-2];
        trim(outPort);
        if(outputPortName.count(outPort)){
            int posOutVec=outgateOrder[outPort];
            out2Gate[posOutVec]=gateId;
            gateId2Vec[gateId]=posOutVec;
            outputGates.insert(gateId);
        }

        inNum[gateId]=lens-2;

        for(int i=0;i<lens-2;i++){
            // cout<<gateId<<" : "<<inputPort[gateId].size()<<endl;
            inputPort[gateId].push_back(-1);

            string inPort=ret[i];
            trim(inPort);
            if(inputPortName.count(inPort)){
                int posInVec=ingateOrder[inPort];
                in2Gate[posInVec].push_back(gateId);
                inputGates.insert(gateId);
                // cout<<posInVec<<"  "<<inputPort<<"  "<<gateId<<endl;
            }
            if(keyPortName.count(inPort)){
                int posInVec=ingateOrder[inPort];
                in2Gate[posInVec].push_back(gateId);
                keyGates.insert(gateId);
                // cout<<posInVec<<"  "<<inputPort<<"  "<<gateId<<endl;
            }
        }
    }
    LogInfoFile.close();
}

string LockedCircuit::keySim(string inVec,string keyVec){
    // vector<vector<int>> matr=g.adj;
    // for(int i=0;i<matr.size();i++){
    //     for(int j=i+1;j<matr[i].size();j++){
    //         if(matr[i][j]==1){
    //             matr[j][i]=1;
    //         }
    //     }
    // }
    // if (isConnected(matr)) {
    //     std::cout << "The graph is connected." << std::endl;
    // } else {
    //     std::cout << "The graph is not connected." << std::endl;
    // }

    //******************************************************
    // for(int i=0;i<g.adj.size();i++){
    //     for(int j=0;j<g.adj[0].size();j++){
    //         log<<g.adj[i][j]<<" ";
    //     }
    //     log<<endl;
    // }
    //******************************************************

    // string line="";

    // for(int i=0;i<function.size();++i){
    //     cout<<i<<":  "<<function[i]<<endl;
    // }


    // cout<<"input size: "<<orderInGate.size()-keySize<<"   key size: "<<keySize<<"   output size: "<<orderOutGate.size()<<endl;
    // cin>>inVec;
    //inVec="10111001010111010101010111111001110101011";
    // cin>>keyVec;
    //keyVec="10101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010";
    vector<vector<int>> newInputPort(inputPort.size());
    for(int i=0;i<inputPort.size();i++){
        vector<int> tempVec(inputPort[i].size(),-1);
        newInputPort[i]=tempVec;
        // for(int j=0;j<inputPort[i].size();j++){
        //     inputPort[i][j]=-1;
        // }
    }

    vector<int> outPort(nodeNums,-1);

    string inputVec=inVec+keyVec;
    // cout<<inputVec.size()<<endl;
    // cout<<inputVec<<endl;
    for(int i=0;i<inputVec.size();i++){
        vector<int> gateIds=in2Gate[i];
        for(int x=0;x<gateIds.size();x++){
            int gateId=gateIds[x];
            for(int k=0;k<inNum[gateId];k++){
                if(newInputPort[gateId][k]==-1){
                    newInputPort[gateId][k]=int(inputVec[i]-'0');
                    break;
                }
            }
        }
    }
    vector<bool> visited(nodeNums,false);
    vector<int> inDegree(nodeNums);
    int visitedNum=0;
    for(int j=0;j<nodeNums;j++){
        int degree=0;
        for(int i=0;i<nodeNums;i++){
            if(g.adj[i][j]==1){
                ++degree;
            }
        }
        inDegree[j]=degree;
    }

    while(1){
        vector<int> zeros;
        for(int i=0;i<nodeNums;i++){
            if(inDegree[i]==0&&!visited[i]){
                zeros.push_back(i);
            }
        }
        // cout<<"zeros "<<zeros.size()<<endl; 
        if(zeros.size()==0){
            break;
        }
        for(int i=0;i<zeros.size();i++){
            int gateId=zeros[i];
            int inNums=inNum[gateId];
            for(int x=0;x<inNums;x++){
                if(newInputPort[gateId][x]==-1){
                    cout<<"no input ports_1 "<<gateId<<endl;
                    return 0;
                }
            }

            int fun=function[gateId];
            //3:xor  4:xnor  5:and  6:or  7:nand  8:nor  9:inv  10:buf
            if(fun==3){   
                // if(inNums!=2){
                //     cout<<"ERROR_3"<<endl;
                // }
                // outPort[gateId]=(inputPort[gateId][0]==inputPort[gateId][1]?0:1);

                outPort[gateId]=newInputPort[gateId][0];
                for(int x=1;x<inNums;x++){
                    outPort[gateId]=(outPort[gateId]==newInputPort[gateId][x]?0:1);
                }
            }
            if(fun==4){
                // if(inNums!=2){
                //     cout<<"ERROR_4"<<endl;
                // }
                // outPort[gateId]=(inputPort[gateId][0]==inputPort[gateId][1]?1:0);    

                outPort[gateId]=newInputPort[gateId][0];
                for(int x=1;x<inNums;x++){
                    outPort[gateId]=(outPort[gateId]==newInputPort[gateId][x]?1:0);
                }
            }
            if(fun==5){
                outPort[gateId]=1;
                for(int x=0;x<inNums;x++){
                    if(newInputPort[gateId][x]==0){
                        outPort[gateId]=0;
                        break;
                    }
                }
            }
            if(fun==6){
                outPort[gateId]=0;
                for(int x=0;x<inNums;x++){
                    if(newInputPort[gateId][x]==1){
                        outPort[gateId]=1;
                        break;
                    }
                }
            }
            if(fun==7){
                outPort[gateId]=1;
                for(int x=0;x<inNums;x++){
                    if(newInputPort[gateId][x]==0){
                        outPort[gateId]=0;
                    }
                }
                outPort[gateId]=(outPort[gateId]+1)%2;  
            }
            if(fun==8){
                outPort[gateId]=0;
                for(int x=0;x<inNums;x++){
                    if(newInputPort[gateId][x]==1){
                        outPort[gateId]=1;
                    }
                }
                outPort[gateId]=(outPort[gateId]+1)%2;  
            }
            if(fun==9){
                if(inNums!=1){
                    cout<<"ERROR_9 "<<inNums<<" "<<gateId<<endl;
                }
                outPort[gateId]=(newInputPort[gateId][0]+1)%2;
            }
            if(fun==10){
                if(inNums!=1){
                    cout<<"ERROR_10"<<endl;
                }
                outPort[gateId]=newInputPort[gateId][0];
            }
            visitedNum++;
            visited[gateId]=true;

            vector<int> nextNodes;
            for(int x=0;x<g.adj[gateId].size();x++){
                if(g.adj[gateId][x]!=0){
                    nextNodes.push_back(x);
                }
            }
            // cout<<"next num:"<<nextNodes.size()<<endl;;
            for(int x=0;x<nextNodes.size();x++){
                int id=nextNodes[x];
                // cout<<"id: "<<id<<endl;
                // cout<<"ingree: "<<inDegree[id]<<endl;
                inDegree[id]--;
                for(int v=0;v<inNum[id];v++){
                    if(newInputPort[id][v]==-1){
                        newInputPort[id][v]=outPort[gateId];
                        break;
                    }
                }
            }
        }
    }
    
    // for(int i=0;i<inDegree.size();i++){
    //     cout<<inDegree[i]<<" ";
    //     if((i+1)%20==0){
    //         cout<<endl;
    //     }
    // }
    // if(visitedNum<nodeNums){
    //     cout<<"fail  "<<visitedNum<<endl;
    // }else{
    //     cout<<"successed"<<endl;
    // }

    vector<string> gateOutName;
    string outVec="";
    map<int, int>::iterator iter;
    for (iter = out2Gate.begin(); iter != out2Gate.end(); ++iter) 
    {
        // cout << iter->first << " => " << outPort[iter->second] << '\n';
        outVec.push_back(outPort[iter->second] + '0');
        gateOutName.push_back(orderOutGate[iter->first]);
    }

    // for(int i=0;i<gateOutName.size();i++){
    //     cout<<gateOutName[i]<<": "<<outPort[i]<<endl;
    // }
    return outVec;
}