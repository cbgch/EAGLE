#include<bits/stdc++.h>

using namespace std;

void trim(string& s)
{
    while(s.back()==' '||s.back()=='\t'||s.back()=='\n') s.pop_back();
    while(s[0]==' '||s[0]=='\t'||s[0]=='\n') s=s.substr(1);
}

std::vector<std::string> split(const std::string &str, const std::string &delimiter = " ") {
    std::vector<std::string> tokens;
    size_t pos = 0, last_pos = 0;
    while ((pos = str.find(delimiter, last_pos)) != std::string::npos) {
        std::string token = str.substr(last_pos, pos - last_pos);
        trim(token);
        if (!token.empty()) {
            tokens.push_back(token);
        }
        last_pos = pos + delimiter.size();
    }
    std::string last_token = str.substr(last_pos);
    trim(last_token);
    if (!last_token.empty()) {
        tokens.push_back(last_token);
    }
    return tokens;
}

int main(int argc, char* argv[]){
    if(argc!=2){
        cout<<"Parameter number error"<<endl;
        return -1;
    }
    vector<vector<double>> predResult; 
    vector<int> ids;                  
    map<int,int> id2KeyBit;
    vector<int> preAns;
    vector<int> realAns;

    int keySize=64;    
    double threshold=0.5;

    vector<int> keys(keySize,-1);
    vector<int> rightKey(keySize,0);

    fstream file_log;
    file_log.open("GNNout/log.txt",ios::in); 
    string line="";
    while(getline(file_log,line)){
        trim(line);
        if(line==""||line.size()==0){
            break;
        }
        vector<string> ret=split(line);
        double d1= stod(ret[0]);
        double d2= stod(ret[1]);
        predResult.push_back({d1,d2});
    }
    file_log.close();

    fstream file_feat_info;
    file_feat_info.open("GNNout/log_feat_info.txt",ios::in);
    while (getline(file_feat_info,line))
    {
        trim(line);
        if(line==""||line.size()==0){
            break;
        }
        getline(file_feat_info,line);
        getline(file_feat_info,line);
        trim(line);
        ids.push_back(stoi(line));
        /* code */
    }
    string circuit=argv[1];
    fstream file_map;
    file_map.open("GNNout/keyMap_"+circuit+".txt",ios::in);
    while(getline(file_map,line)){
        trim(line);
        if(line==""||line.size()==0){
            break;
        }
        vector<string> ret=split(line);
        trim(ret[0]);
        trim(ret[1]);
        int id=stoi(ret[1]);
        int keyId=stoi(ret[0].substr(8));
        id2KeyBit[id]=keyId;
    }

    fstream file_ans;
    file_ans.open("GNNout/ans.txt",ios::in);
    int tempCounter=0;
    while(getline(file_ans,line)){
        trim(line);
        if(line==""||line.size()==0){
            break;
        }
        vector<string> ret=split(line);
        realAns.push_back(stoi(ret[0]));
        preAns.push_back(stoi(ret[1]));
        rightKey[id2KeyBit[ids[tempCounter++]]]=realAns.back();
    }

    vector<int> keyGNN(keySize,-1);
    vector<double> maxKeyScore(keySize,0.0);

    vector<int> checkId;
    int rightCount=0;
    int crashCount=0; 
    for(int i=0;i<predResult.size();++i){
        int curId=ids[i];   
        double res=max(abs(predResult[i][0]),abs(predResult[i][1]));
        int curKeyBit=id2KeyBit[curId];
        if(res>threshold){
            checkId.push_back(curId);
            if(keys[curKeyBit]!=-2){
                if(keys[curKeyBit]==-1){
                    keys[curKeyBit]=preAns[i];                    
                }else{                                            
                    if(keys[curKeyBit]!=preAns[i]){
                        keys[curKeyBit]=-2;
                    }
                }
            }
        }

        if(res>maxKeyScore[curKeyBit]){
            keyGNN[curKeyBit]=preAns[i];
            maxKeyScore[curKeyBit]=res;
        }
    }

    int allWrongBit=0;
    int undefBit=0;
    for(int i=0;i<keyGNN.size();i++){
        if(keyGNN[i]==-1||keyGNN[i]==-2){
            undefBit++;
        }else{
            if(keyGNN[i]!=rightKey[i]){
                allWrongBit++;
            }
        }
    }

    fstream outPutFile;
    outPutFile.open("result.txt",ios::out);
    outPutFile<<"key-size: "<<keySize<<endl;

    outPutFile<<"realKey:          ";
    for(int bit:rightKey){
        cout<<bit;
        outPutFile<<bit;
    }
    cout<<endl;
    outPutFile<<endl;

    int undefGA=0;
    int wrongGA=0;
    outPutFile<<"the key to phase: ";
    for(int xx=0;xx<keySize;xx++){
        int bit=keys[xx];
        if(bit==-1||bit==-2){
            cout<<"x";
            outPutFile<<"x";
            ++undefGA;
        }else{
            cout<<bit;
            outPutFile<<bit;
            if(bit!=rightKey[xx]){
                ++wrongGA;
            }
        }
    }
    cout<<endl;
    outPutFile<<endl;

    outPutFile<<"GNN predict:      ";
    for(int bit:keyGNN){
        if(bit==-1||bit==-2){
            cout<<"x";
            outPutFile<<"x";
        }else{
            cout<<bit;
            outPutFile<<bit;
        }
    }
    cout<<endl;
    outPutFile<<endl;


    outPutFile<<"Ku: "<<undefGA<<endl;
    outPutFile<<"Wb: "<<wrongGA<<endl;

    double kpa=((double)(keySize-allWrongBit))/((double)keySize);
    outPutFile<<"KPA: "<<kpa;
}