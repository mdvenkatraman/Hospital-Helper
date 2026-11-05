#pragma once
#include "Point.h"
#include<vector>
#include<cstddef>

struct KDNode{ // a node of the KD-tree where each node contains one hospital
    Hospital hospital;
    int axis = 0; //which coordinate this node splits on: 0 = latitude, 1 = longitude (set by depth % 2)
    KDNode* left = nullptr; //hospitals with smaller value on the axis
    KDNode* right = nullptr; //hospitals with larger value on the axis


};

class KD_Tree{
    private:
    KDNode* root;
    std::size_t count; //number of hospitals

    KDNode* build(std:: vector<Hospital>& pts, std::size_t fi, std:: size_t la, int depth); // builds the tree from pts[fi, la) 
    // returns the root

    void search(const KDNode* n, double latitude, double longitude, const Hospital*& nearest, double& best) const;
    // recursive search for nearest neighbour and updating the nearest and best variables
    public: 
        KD_Tree();
        void build(std::vector<Hospital> pts);

        const Hospital* nearest(double latitude, double longitude) const;
};

