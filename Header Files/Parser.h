#pragma once
#include "Point.h"
#include "KD-Tree.h"
#include "RTree.h"
#include <vector>
#include <string>

struct BlockGroup {
    std::string id;
    double latitude;
    double longitude;
};

class DataParser {
public:
    // Parses hospital CSV and populates both trees and a flat vector
    static bool parseHospitals(const std::string& filename, KD_Tree& kdTree, RTree& rTree, std::vector<Hospital>& outHospitals);

    // Parses US Census Block Groups CSV
    static bool parseBlockGroups(const std::string& filename, std::vector<BlockGroup>& outBlockGroups);
};