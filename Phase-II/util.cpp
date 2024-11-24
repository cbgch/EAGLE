#include "util.h"
#include <bits/stdc++.h>
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