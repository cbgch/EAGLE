#ifndef GRAPH_H
#define GRAPH_H

#include<bits/stdc++.h>
using namespace std;
class Graph {
public:
    Graph(){}
    Graph(int vertices);
    void addEdge(int v, int w);
public:
    int vertices; 
    std::vector<std::vector<int>> adj;
};

#endif
