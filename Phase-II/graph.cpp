#include<bits/stdc++.h>
#include "graph.h"

Graph::Graph(int vertices) {
    this->vertices = vertices;
    adj=std::vector<std::vector<int>>(vertices,std::vector<int>(vertices,0));
}

void Graph::addEdge(int v, int w) {
    adj[v][w]=1;
}