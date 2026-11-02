#pragma once

#include "Point.h"
#include "Rect.h"
#include <vector>
#include <iostream>
#include <limits>

struct Node {
    bool isLeaf = true;
    //Internal nodes hold other nodes, leaves hold points
    Rect mbr; //minimum bounding rectangle, the smallest rectangle that stores the item(s)
    std::vector<Node*> children; //children of the node, used for internal nodes
    std::vector<Hospital> points; //points of the node, used for leaf nodes
};

class RTree {
    private:
        Node* root;
        void destroy(Node* n);
        void search (const Node* n, double lat, double lon, const Hospital*& best, double& bestSq) const;

    public:
        RTree();

        ~RTree();

        void build(std::vector<Hospital> pts);
        
        //returns a pointer to the nearest hospital
        const Hospital* nearest (double lat, double lon) const;
};