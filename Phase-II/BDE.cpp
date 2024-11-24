#include<bits/stdc++.h>
#include "omp.h"
#include "graph.h"
#include "oracle.h"
#include "lockedCircuit.h"
using namespace std;

vector<double> outPutWeight;
int maxWeight=0;
int totalWeight=0;

double calHammDist(string str1, string str2){
    double ret=0;
    if(str1.size()!=str2.size()){
        cout<<"different length"<<endl;
        return -1;
    }
    int n=str1.size();
    for(int i=0;i<n;i++){
        if(str1[i]!=str2[i]){
            ret=ret+outPutWeight[i];
            // ++ret;
        }
    }
    // cout<<ret<<endl;
    return ret;
}

double calFitness(string key,const vector<string>& inputPatterns,const vector<string>& inputPatternOut,Oracle& oracle, LockedCircuit& lockedCircuit){
    string inputStr, keyStr;
    // keyStr="1111000011101101011001010111101001111011111100110000001011100100";
    // inputStr="00000011111001110111000011111111";

    double ret=0.0;
    int testNum=inputPatterns.size();
    for(int i=0;i<testNum;++i){
        // cout<<"times: "<<i<<endl;
        keyStr=key;
        inputStr=inputPatterns[i];
        string realRet = inputPatternOut[i];
        string keyRet = lockedCircuit.keySim(inputStr,keyStr);
        // cout<<"keyRet:  "<<keyRet<<endl;
        // cout<<"realRet: "<<realRet<<endl;

        int n=keyRet.size();
        for(int i=0;i<n;i++){
            if(realRet[i]==keyRet[i]){
                ret=ret+outPutWeight[i];
                // ++ret;
            }
        }
    }
    return ret;
}

string combineKey(string key1, string key2){
    int n=key1.size();
    int pos=0;
    for(int i=0;i<n;i++){
        if(key1[i]=='x'){
            key1[i]=key2[pos++];
        }
    }
    return key1;
}


// std::string generateRandomBinaryString(int strSize) {
//     std::string binaryString;
//     for (int i = 0; i < strSize; ++i) {
//         binaryString += (rand() % 2) ? '1' : '0';
//     }
//     return binaryString;
// }

// vector<vector<int>> generateString(int populationSize, int numCities) {
//     vector<vector<int>> population(populationSize, vector<int>(numCities));
//     for (auto& individual : population) {
//         // iota(individual.begin(), individual.end(), 0); // 填充0到numCities-1
//         for(int i=0;i<individual.size();++i){
//             individual[i]=i;
//         }
//         random_shuffle(individual.begin(), individual.end());
//     }
//     return population;
// }


string strXor(const string& str1, const string& str2) {
    string result=str1;
    size_t len = str1.size();
    if(str2.size()!=len){
        return "";
    }

    for (size_t i = 0; i < len; ++i) {
        result[i]=str1[i]==str2[i]?'0':'1';
    }

    return result;
}

string strXnor(const string& str1, const string& str2) {
    string result=str1;
    size_t len = str1.size();
    if(str2.size()!=len){
        return "";
    }

    for (size_t i = 0; i < len; ++i) {
        result[i]=str1[i]==str2[i]?'1':'0';
    }

    return result;
}


string strOr(const string& str1, const string& str2) {
    string result=str1;
    size_t len = str1.size();
    if(str2.size()!=len){
        return "";
    }

    for (size_t i = 0; i < len; ++i) {
        if(str1[i]=='1'||str2[i]=='1'){
            result[i]=='1';
        }else{
            result[i]=='0';
        }
    }

    return result;
}

string strAnd(const string& str1, const string& str2) {
    string result=str1;
    size_t len = str1.size();
    if(str2.size()!=len){
        return "";
    }

    for (size_t i = 0; i < len; ++i) {
        if(str1[i]=='1'&&str2[i]=='1'){
            result[i]=='1';
        }else{
            result[i]=='0';
        }
    }

    return result;
}

string strNot(string& str1){
    string result=str1;
    size_t len = str1.size();

    for (size_t i = 0; i < len; ++i) {
        if(str1[i]=='1'){
            result[i]=='0';
        }else{
            result[i]=='1';
        }
    }
    return result;
}

//************************************************************************************************************* */
string generateBinaryString(int len) {
    string result;
    for (int i = 0; i < len; ++i) {
        result += (rand() % 2 == 0) ? '0' : '1';
    }
    return result;
}

vector<string> generateString(int populationSize, int keySize){
    set<string> uniqueStrings;
    while (uniqueStrings.size() < populationSize) {
        uniqueStrings.insert(generateBinaryString(keySize));
    }
    return vector<string>(uniqueStrings.begin(), uniqueStrings.end());
}
//************************************************************************************************************* */


void crossover(int j,double CR,vector<string>& population_Xi, vector<string>& population_Ui) {
    int NP=population_Xi.size();
    float p=rand() % (10000) / (float)(10000);
    if(p>CR){
        for(int i=0;i<NP;++i){
            population_Ui[i][j]=population_Xi[i][j];
        }
    }
}

void mutate(int i,int pattern,vector<string>& population_Xi, vector<string>& population_Ui,string x_best) {
    int NP=population_Xi.size();

    unordered_set<int> random_numbers;

    while (random_numbers.size() < 5) {
        int num = rand()%NP;
        if (num != i) {
            random_numbers.insert(num);
        }
    }
    auto it = random_numbers.begin();
    int pos1 = *it++;
    int pos2 = *it++;
    int pos3 = *it++;
    int pos4 = *it++;
    int pos5 = *it++;

    if(pattern==0){
        population_Ui[i]=strXor(population_Xi[pos1],strXnor(population_Xi[pos2],population_Xi[pos3]));
    }
    if(pattern==1){
        population_Ui[i]=strXor(x_best,strXnor(population_Xi[pos1],population_Xi[pos2]));
    }
    if(pattern==2){
        population_Ui[i]=
            strXor(population_Xi[pos1],strXor(strOr(population_Xi[pos2],population_Xi[pos3]),strAnd(population_Xi[pos4],population_Xi[pos5])));
    }
    if(pattern==3){
        population_Ui[i]=
            strXor(x_best,strXor(strOr(population_Xi[pos1],population_Xi[pos2]),strAnd(population_Xi[pos3],population_Xi[pos4])));
    }
    if(pattern==4){
        population_Ui[i]=
            strXor(population_Xi[i],strOr(strXnor(x_best,population_Xi[i]),strXnor(population_Xi[pos1],population_Xi[pos2])));
    }
    if(pattern==5){
        population_Ui[i]=
            strXnor(population_Xi[i],strAnd(strXor(x_best,population_Xi[i]),strXor(population_Xi[pos1],population_Xi[pos2])));
    }
}

void printPairVector(const std::vector<std::pair<double, std::string>>& vec) {
    for (const auto& p : vec) {
        std::cout << "(" << p.first << ", " << p.second << ")" << std::endl;
    }
}

vector<string> chooseTestInput(const vector<string>& tempTestPattern,const string& key, 
            const string& realKey,const vector<string>& population, Oracle& oracle,LockedCircuit& lockedCircuit){
    int n=tempTestPattern.size();
    int NP=population.size();
    vector<pair<double,string>> vec(n);
    for(int i=0;i<n;++i){
        string realRet = oracle.oracleSim(tempTestPattern[i]);
        double score=0.0;
        omp_set_num_threads(96);
        #pragma omp parallel for
        for(int j=0;j<NP;++j){
            string curKey=combineKey(key,population[j]);
            string keyRet = lockedCircuit.keySim(tempTestPattern[i],curKey);
            // cout<<"keyRet:  "<<keyRet<<endl;
            // cout<<"realRet: "<<realRet<<endl;
            int diff=calHammDist(realRet,keyRet);
            score+=diff;
        }
        vec[i]=make_pair(score, tempTestPattern[i]);
    }
    sort(vec.begin(), vec.end(), [](const pair<double, string>& a, const pair<double, string>& b) {
        return a.first > b.first;
    });

    // printPairVector(vec);

    int m=n/3;
    vector<string> retPattern(m);
    int pos=0;
    for (const auto& p : vec) {
        retPattern[pos++]=p.second;
        if(pos>=m){
            break;
        }
    }
    return retPattern;
}

void DFS(const vector<vector<int>>& adjMatrix, vector<bool>& visited, int node,
            vector<int>& outputWeight,const set<int>& outputGates, const map<int,int>& gateId2Vec) {
    visited[node] = true;

    if(outputGates.count(node)){
        int vecOrder=gateId2Vec.at(node);
        ++outputWeight[vecOrder];
    }

    for (int i = 0; i < adjMatrix.size(); ++i) {
        if (adjMatrix[node][i] == 1 && !visited[i]) {
            DFS(adjMatrix, visited, i,outputWeight,outputGates,gateId2Vec);
        }
    }
}

void calOutputWeight(const LockedCircuit& lockedCircuit, vector<double>& outputWeight){
    int numNodes=lockedCircuit.nodeNums;
    int outn=outputWeight.size();

    vector<int> keyConnectNum(outn,0);
    vector<int> PiConnectNum(outn,0);

    for (auto it = lockedCircuit.keyGates.begin(); it != lockedCircuit.keyGates.end(); ++it) {
        // std::cout << *it << " ";
        int startNode=*it;
        vector<bool> visited(numNodes, false);
        DFS(lockedCircuit.g.adj, visited, startNode,keyConnectNum,lockedCircuit.outputGates,lockedCircuit.gateId2Vec);
    }

    for (auto it = lockedCircuit.inputGates.begin(); it != lockedCircuit.inputGates.end(); ++it) {
        // std::cout << *it << " ";
        int startNode=*it;
        vector<bool> visited(numNodes, false);
        DFS(lockedCircuit.g.adj, visited, startNode,PiConnectNum,lockedCircuit.outputGates,lockedCircuit.gateId2Vec);
    }
    for(int i=0;i<outn;i++){
        cout<<keyConnectNum[i]<<endl;
    }
    cout<<"--------------------------------------------------------------------"<<endl;
    for(int i=0;i<outn;i++){
        cout<<PiConnectNum[i]<<endl;
    }
    cout<<"--------------------------------------------------------------------"<<endl;
    for(int i=0;i<outn;i++){
        outputWeight[i]=(double(keyConnectNum[i]))/(double(keyConnectNum[i]+PiConnectNum[i]));
    }
}

int main(int argc, char* argv[]) {
    time_t start = time(nullptr);
    fstream ftime;
    ftime.open("timeRecord.txt",ios::out);
    
    fstream file;
    file.open("result.txt",ios::out);

    if(argc!=3){
        cout<<"Parameter number error"<<endl;
        return -1;
    }
    
    time_t seeds=time(0);
    srand(seeds);

    string lock = argv[1];
    string circuit=argv[2];

    Oracle oracle=Oracle(lock,circuit);
    oracle.initOracle();

    LockedCircuit lockedCircuit=LockedCircuit(lock,circuit);
    lockedCircuit.initCircuit();

    int inputSize = 0, outputSize = 0, keySize = 0;
    string realKey, key;

    ifstream conf(lock+"/"+circuit+"/conf.txt");
    if (!conf.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return 1;
    }

    string line;
    while (getline(conf, line)) {
        istringstream iss(line);
        string keyName;
        string value;

        if (getline(iss, keyName, ':') && getline(iss >> ws, value)) {
            if (keyName == "inputSize") {
                inputSize = stoi(value);
            } else if (keyName == "outputSize") {
                outputSize = stoi(value);
            } else if (keyName == "keySize") {
                keySize = stoi(value);
            } else if (keyName == "realKey") {
                realKey = value;
            } else if (keyName == "key") {
                key = value;
            }
        }
    }
    conf.close();
  
    cout<<inputSize<<endl;
    cout<<outputSize<<endl;
    cout<<keySize<<endl;
    cout<<realKey<<endl;
    cout<<key<<endl;
    
    int numInputPatterns;  

    int testInputPatternNum=300;   

    int patternNum=6;  

    int NP = 26;  
    int generations = 2000;   

    const float CR = 0.5; 


    int uncertainNum = count(key.begin(),key.end(),'x');
    cout<<"the number of uncertain bits: "<<uncertainNum<<endl;

    outPutWeight.resize(outputSize,0.0);
    for(int i=0;i<outPutWeight.size();++i){
        outPutWeight[i]=0.0;
    }
    calOutputWeight(lockedCircuit,outPutWeight);
    for(int i=0;i<outPutWeight.size();i++){
        cout<<outPutWeight[i]<<endl;
    }

    auto population_Xi = generateString(NP, uncertainNum);
    vector<string> population_Ui(NP);

    vector<string> tempTestPattern=generateString(testInputPatternNum*3,inputSize);   //1000

    vector<string> testPattern=chooseTestInput(tempTestPattern,key,realKey,population_Xi,oracle,lockedCircuit);

    vector<string> testPatternOutPut(testInputPatternNum);

    omp_set_num_threads(96);
    #pragma omp parallel for
    for(int i=0;i<testInputPatternNum;++i){
        testPatternOutPut[i]=oracle.oracleSim(testPattern[i]);
    }
    cout<<"out vector weight finish"<<endl;


/////////////////////////////////////////////////////////////////////////////////////////
    vector<double> fitness_Xi(NP);
    vector<double> fitness_Ui(NP);

    set<string> uniqueSet;

    for(int i=0;i<NP;++i){
        uniqueSet.insert(population_Xi[i]); 
    }

    omp_set_num_threads(96);
    #pragma omp parallel for
    for (int i = 0; i < NP; ++i) {
        string curKey=combineKey(key,population_Xi[i]);
        double fit= calFitness(curKey,testPattern,testPatternOutPut,oracle,lockedCircuit);
        fitness_Xi[i]=fit;
    }

    string x_best="";
    double maxFitness=-1.0;
    for(int i=0;i<NP;++i){
        if(fitness_Xi[i]>maxFitness){
            maxFitness=fitness_Xi[i];
            x_best=population_Xi[i];
        }
    }

    for (int generation = 0; generation < generations; ++generation) {
        cout<<"generation: "<<generation<<endl;
        file<<"generation: "<<generation<<"   ";

        for(int i=0;i<NP;i++){
            int pattern=rand()%patternNum;
            mutate(i,pattern,population_Xi,population_Ui,x_best);
        }

        for(int j=0;j<uncertainNum;j++){
            crossover(j,CR,population_Xi,population_Ui);
        }


        map<string, double> hs;
        for (int i = 0; i < NP; ++i) {
            hs[population_Ui[i]]=0.0;
        }

        vector<string> uniquePopulation(hs.size());
        int uniquePos=-1;
        map<string, double>::iterator iter;
        iter = hs.begin();
        while(iter != hs.end()){
            uniquePopulation[++uniquePos]=iter->first;
            iter++;
        }
        int uNum=uniquePopulation.size();

        omp_set_num_threads(96);
        #pragma omp parallel for
        for (int i = 0; i < uNum; ++i) {
            string curKey=combineKey(key,uniquePopulation[i]);
            double fit= calFitness(curKey,testPattern,testPatternOutPut,oracle,lockedCircuit);
            // fitness_Ui[i]=fit;
            hs[uniquePopulation[i]]=fit;
        }

        for(int i=0;i<NP;++i){
            fitness_Ui[i]=hs[population_Ui[i]];
        }

        for(int i=0;i<NP;++i){
            if(fitness_Ui[i]>fitness_Xi[i]){
                fitness_Xi[i]=fitness_Ui[i];
                population_Xi[i]=population_Ui[i];
                if(fitness_Ui[i]>maxFitness){
                    maxFitness=fitness_Ui[i];
                    x_best=population_Xi[i];
                }
            }
        }
       
        vector<int> wrongBit(NP);
        for(int i=0;i<NP;++i){
            int countBit=0;
            string outKey=combineKey(key,population_Xi[i]);
            for(int j=0;j<realKey.size();++j){
                if(realKey[j]!=outKey[j]){
                    ++countBit;
                }
            }
            wrongBit[i]=countBit;
        }
        
        
        int maxFitWrongBit=0;
        string maxFitKey=combineKey(key,x_best);
        for(int j=0;j<realKey.size();++j){
            if(realKey[j]!=maxFitKey[j]){
                ++maxFitWrongBit;
            }
        }

        file<<"best key: "<<x_best<<"    accuracy: "<<1-((double)maxFitWrongBit)/((double)keySize)<<endl;
        
        time_t end = time(nullptr);
        int elapsed = static_cast<int>(difftime(end, start));
        ftime<<elapsed << ":  "<<x_best<<"   accuracy: "<<1-((double)maxFitWrongBit)/((double)keySize)<<endl;
    }

    return 0;
}