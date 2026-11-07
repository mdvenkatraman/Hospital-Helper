#include "KD-Tree.h"
#include <algorithm>
#include <limits>
#include <utility>
KD_Tree::KD_Tree() : root(nullptr), count(0) {}

KD_Tree:: ~KD_Tree() {
    destroy(root);
}
void KD_Tree:: build(std::vector<Hospital> pts){
    destroy(root);
    root = nullptr;
    count = pts.size();
    root = build_tree(pts, 0, pts.size(), 0);
}

KDNode* KD_Tree::build_tree(std::vector<Hospital>& pts, std::size_t fi, std::size_t la, int depth) {
    if (fi>= la){
        return nullptr; //no hospitals 
    }
    int axis = depth % 2; 
    std:: size_t mid = fi + (la - fi)/2;
    // the median would come to index mid, the smaller values before and larger after
    selectMedian(pts, fi, la, mid, axis);
    KDNode* node = new KDNode();
    node->hospital = pts[mid];
    node->axis = axis;
    node->left = build_tree(pts, fi, mid, depth+1);
    node->right = build_tree(pts, mid+1, la, depth +1);
    return node;

}


void KD_Tree:: search(const KDNode* n, double latitude, double longitude, const Hospital*& nearest, double& best) const {
    if (n ==nullptr) return;

    double difference_latitude = n->hospital.latitude - latitude;
    double difference_longitude = n->hospital.longitude - longitude;
    double distance = difference_latitude*difference_latitude + difference_longitude* difference_longitude;
    if (distance <best){
        best = distance;

        nearest = &n->hospital;
    }
    //checking the axis to decide what coordinate matters at the node
    double query_val;
    if (n->axis == 0){
        query_val = latitude;

    }
    else{
        query_val = longitude;
    }

    double diff = query_val - coord(n->hospital, n->axis);
    const KDNode* near_side;
    const KDNode* far_side;
    if (diff <0){
        near_side = n->left; //query is smaller so it's on left
        far_side = n->right;
    } else{
        near_side = n->right; //query is bigger or equal so its on right
        far_side = n->left;
    }
    search(near_side, latitude, longitude, nearest,best);
    if (diff*diff < best){
        search(far_side, latitude, longitude, nearest, best);
        }
    


}
const Hospital* KD_Tree::nearest(double latitude, double longitude) const {
    const Hospital* nearest = nullptr;
    double best = std::numeric_limits<double>::max();
    search(root, latitude, longitude, nearest, best);
    return nearest;
}
std::size_t KD_Tree::size() const {
    return count;
}
int KD_Tree::height_node(const KDNode* n) const {
    if (n == nullptr) return 0;
    return 1+ std::max(height_node(n->left), height_node(n->right));

}
int KD_Tree:: height() const {
    return height_node(root);
}

void KD_Tree::selectMedian(std::vector<Hospital>& pts, std::size_t fi, std::size_t la, std::size_t k, int axis){
    while (la - fi >1) {
        std::size_t p = partition (pts, fi, la, axis); //splits the vector into two sides around middle
        if (p == k)return; //index k is the target postion 
        if (k<p) la = p; 
        else fi = p+1;

    }
}
std::size_t KD_Tree::partition(std::vector<Hospital>& pts, std::size_t fi, std::size_t la, int axis){
    std::size_t mid = fi+ (la - fi) /2; // considering mid as pivot for now
    std::swap (pts[mid], pts[la-1]); // moving the mid element at the end

    double pivotVal = coord(pts[la-1], axis);

    std::size_t store = fi;
    // this loop will move everything smaller than the pivot to the front
    for (std::size_t i = fi; i<la -1; i++){
        if (coord(pts[i], axis)< pivotVal){
            std:: swap(pts[i], pts[store]);
            store++;
        }
    }
    std:: swap(pts[store], pts[la-1]); //puts the pivot right after the smaller element

    return store;

}


void KD_Tree::destroy(KDNode* n) {
    if (n == nullptr) return;
    destroy(n->left);
    destroy(n->right);
    delete n;
}

double KD_Tree::coord(const Hospital& h, int axis) {
    if (axis == 0) return h.latitude;
    return h.longitude;
}