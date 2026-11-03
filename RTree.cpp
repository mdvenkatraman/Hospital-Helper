#include "RTree.h"
#include "Point.h"
#include <cmath>
#include <limits>
#include <utility>
#include <algorithm>


void RTree::destroy(Node* n) {
    if (n == nullptr) return;
    for (Node* c : n->children) {
        destroy(c);
    }
    delete n;
}

RTree::RTree() {
    root = new Node();
}

RTree::~RTree() {
    destroy(root);
} 

void RTree::build(std::vector<Hospital> pts) {
    destroy(root);
    root = nullptr;
    if (pts.empty()) {
        root = new Node();
        return;
    }

    //sort by longitude, slice it, sort slices by latitude, pack the leaves
    std::sort(pts.begin(), pts.end(), [](const Hospital& a, const Hospital& b) {
    return a.longitude < b.longitude;
    });

    std::size_t n = pts.size();
    std::size_t leafCount = (n + M - 1) / M; //ceil(n / M) but for integers
    std::size_t sliceCount = (std::size_t)std::ceil(std::sqrt((double)leafCount));
    std::size_t sliceSize = sliceCount * M;

    std::vector<Node*> level;
    for (std::size_t i=0; i< n; i+=sliceSize) {
        std::size_t e = std::min(i + sliceSize, n);
        std::sort(pts.begin() + i, pts.begin() + e, [](const Hospital& a, const Hospital& b) {
            return a.latitude < b.latitude;
        });

        for (std::size_t j = i; j<e; j+=M) {
            std::size_t k = std::min(j+M, e);
            Node* leaf = new Node();  
            for (std::size_t l=j; l<k; l++) {
                leaf->points.push_back(pts[l]);
                leaf->mbr.expand(Rect::fromPoint(pts[l].latitude, pts[l].longitude));   
            }
            level.push_back(leaf);
        }
    }

    //build the parent levels until we have only one sole root
    while (level.size() > 1) {
        std::vector<Node*> next;
        for (std::size_t i=0; i< level.size(); i+=M) {
            Node* parent = new Node();
            parent->isLeaf = false;
            for (std::size_t j=i; j< std::min(i+M, level.size()); j++) {
                parent->children.push_back(level[j]);
                parent->mbr.expand(level[j]->mbr);
            }
            next.push_back(parent);
        }
        level = next;
    }
    root = level[0];
}

void RTree::search (const Node* n, double lat, double lon, const Hospital*& best, double& bestSq) const {
    if (n->isLeaf) {
        for (const Hospital& h : n ->points) {
            double dLat = h.latitude - lat;
            double dLon = h.longitude - lon;
            double d = dLat * dLat + dLon * dLon;
            if (d < bestSq) {
                bestSq = d;
                best = &h;
            }
        }
        return;
    }
    //visit closest boxes first so bestSq shrinks more quickly
    std::vector<std::pair<double, const Node*>> order;
    for (const Node* c : n->children) {
        order.push_back({c->mbr.minDistSq(lat, lon), c});
    }   
    std::sort(order.begin(), order.end(), [](const auto& a, const auto& b) {
    return a.first < b.first;
    });
    for (const auto& [d, c] : order) {
        if (d >= bestSq) break; //no need to search this box or any further boxes
        search(c, lat, lon, best, bestSq);
    }
}

const Hospital* RTree::nearest (double lat, double lon) const {
    const Hospital* best = nullptr;
    double bestSq = std::numeric_limits<double>::max();
    search(root, lat, lon, best, bestSq);
    return best;
}