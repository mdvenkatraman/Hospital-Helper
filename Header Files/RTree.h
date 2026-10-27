#include "Point.h"

#include <iostream>

struct Rect {
    double minLat, minLon, maxLat, maxLon; //sets edges of rectangles
    void expand (const Rect& other); //expand the current rectangle to include other, min of mins, max of maxes
    double area() const; //compares rectangle sizes
    double enlargementToInclude(const Rect& other) const; //returns the area of the rectangle that would be needed to include other
    double minDist (double lat, double lon) const; //returns the minimum distance from a point to the rectangle, used for min neighbors
};

struct Node {
    bool isLeaf;
    //Internal nodes hold other nodes, leaves hold points
    Rect mbr; //minimum bounding rectangle, the smallest rectangle that stores the item(s)
    std::vector<Node*> children; //children of the node, used for internal nodes
    std::vector<Hospital> points; //points of the node, used for leaf nodes
};