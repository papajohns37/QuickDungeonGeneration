#pragma once

struct Edge {
    int v1;
    int v2;
    float weight;

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};