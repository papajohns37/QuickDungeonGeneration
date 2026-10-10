#pragma once

#include <vector>

class DisjointSet {
    private: 
        std::vector<int> parent;

    public:
        /// @brief Creates a disjoint set with V items.
        /// @param V Number of items in disjoint set.
        DisjointSet(int V);
        /// @brief Finds the parent of an item in the disjoint set.
        /// @param i Item we want to find the parent of.
        /// @return Parent of the item.
        int find(int i);
        /// @brief Combines two sets if they have different roots
        /// @param i Item to unite.
        /// @param j Item to unite.
        /// @return whether items were successfully joined.
        bool unite(int i, int j);
};