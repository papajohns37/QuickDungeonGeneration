#include "DisjointSet.h"

DisjointSet::DisjointSet(int V){
    parent.resize(V);
    for (size_t i = 0; i < parent.size(); i++)
    {
        parent[i] = i;
    }
}

int DisjointSet::find(int i){
    if (parent[i] == i){
        return i;
    }
    return parent[i] = find(parent[i]);
}

bool DisjointSet::unite(int i, int j){
    int root_i = find(i);
    int root_j = find(j);

    if (root_i == root_j){
        return false;
    }

    parent[root_i] = root_j;
    return true;
}