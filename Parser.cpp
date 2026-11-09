#include "Parser.h"
#include <fstream>
#include <sstream>
#include <iostream>


static std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream tokenStream(line);
    while (std::getline(tokenStream, token, ',')) {
        tokens.push_back(token);
    }
    return tokens;
}

bool DataParser::parseHospitals(const std::string& filename, KD_Tree& kdTree, RTree& rTree, std::vector<Hospital>& outHospitals) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open hospital file: " << filename << "\n";
        return false;
    }

    std::string line;
    std::getline(file, line);

    outHospitals.clear();
    while (std::getline(file, line)) {
        std::vector<std::string> fields = parseCSVLine(line);

        if (fields.size() >= 3) {
            Hospital h;
            h.name = fields[0];
            try {
                h.latitude = std::stod(fields[1]);
                h.longitude = std::stod(fields[2]);
                outHospitals.push_back(h);
            } catch (...) {

                continue;
            }
        }
    }
    file.close();

    kdTree.build(outHospitals);
    rTree.build(outHospitals);
    return true;
}

bool DataParser::parseBlockGroups(const std::string& filename, std::vector<BlockGroup>& outBlockGroups) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open block groups file: " << filename << "\n";
        return false;
    }

    std::string line;
    std::getline(file, line);

    outBlockGroups.clear();
    while (std::getline(file, line)) {
        std::vector<std::string> fields = parseCSVLine(line);

        if (fields.size() >= 3) {
            BlockGroup bg;
            bg.id = fields[0];
            try {
                bg.latitude = std::stod(fields[1]);
                bg.longitude = std::stod(fields[2]);
                outBlockGroups.push_back(bg);
            } catch (...) {
                continue;
            }
        }
    }
    file.close();
    return true;
}