#include "RTree.h"
#include "Point.h"
#include <cmath>
#include <limits>
#include <utility>


void RTree::destroy(Node* n) {
    if (n == nullptr) return;
    for (Node* c : n->children) {
        destroy(c);
    }
    delete n;
}

void RTree::search (const Node* n, double lat, double lon, const Hospital*& best, double& bestSq) const {
    //Implement
}

RTree::RTree() {
    root = new Node();
}

RTree::~RTree() {
    destroy(root);
} 

void RTree::build(std::vector<Hospital> pts) {
    //Implement
}

const Hospital* RTree::nearest (double lat, double lon) const {
    //Implement
}