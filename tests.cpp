#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "KD-Tree.h"
#include "RTree.h"
#include "BruteForce.h"

// Helper to generate a controlled set of test hospitals
std::vector<Hospital> getTestHospitals() {
    return {
        {"Hospital A", 25.0, -80.0},
        {"Hospital B", 30.0, -90.0},
        {"Hospital C", 35.0, -100.0},
        {"Hospital D", 40.0, -110.0},
        {"Hospital E", 45.0, -120.0},
        {"Hospital F", 28.0, -82.0},
        {"Hospital G", 32.0, -95.0},
        {"Hospital H", 38.0, -105.0}
    };
}

// ==================== K-D TREE TESTS (~20 Cases) ====================
TEST_CASE("KD-Tree Basic Functionality", "[kd-tree]") {
    auto hospitals = getTestHospitals();
    KD_Tree kdTree;
    kdTree.build(hospitals);

    SECTION("Exact match coordinate query") {
        const Hospital* res = kdTree.nearest(25.0, -80.0);
        REQUIRE(res != nullptr);
        REQUIRE(res->name == "Hospital A");
    }

    SECTION("Query close to Hospital F") {
        const Hospital* res = kdTree.nearest(28.1, -82.1);
        REQUIRE(res != nullptr);
        REQUIRE(res->name == "Hospital F");
    }

    SECTION("Tree size matches input size") {
        REQUIRE(kdTree.size() == 8);
    }
}

TEST_CASE("KD-Tree Edge Cases and Brute Force Validation", "[kd-tree][brute-force]") {
    auto hospitals = getTestHospitals();
    KD_Tree kdTree;
    kdTree.build(hospitals);


    std::vector<std::pair<double, double>> queryPoints = {
        {26.0, -81.0}, {29.5, -89.0}, {34.0, -99.0}, {41.0, -112.0},
        {46.0, -122.0}, {24.0, -79.0}, {50.0, -130.0}, {30.0, -90.0},
        {35.5, -101.0}, {39.0, -109.0}, {44.0, -119.0}, {27.0, -83.0},
        {33.0, -94.0}, {37.0, -104.0}, {42.0, -111.0}, {48.0, -125.0}
    };

    int caseNum = 1;
    for (const auto& q : queryPoints) {
        DYNAMIC_SECTION("Random Query Point " << caseNum++) {
            const Hospital* expected = BruteForce::findNearest(hospitals, q.first, q.second);
            const Hospital* actual = kdTree.nearest(q.first, q.second);
            
            REQUIRE(actual != nullptr);
            REQUIRE(expected != nullptr);
            REQUIRE(actual->name == expected->name);
        }
    }
}

// ==================== R-TREE TESTS (~20 Cases) ====================
TEST_CASE("R-Tree Basic Functionality", "[rtree]") {
    auto hospitals = getTestHospitals();
    RTree rTree;
    rTree.build(hospitals);

    SECTION("Exact match coordinate query") {
        const Hospital* res = rTree.nearest(25.0, -80.0);
        REQUIRE(res != nullptr);
        REQUIRE(res->name == "Hospital A");
    }

    SECTION("Query close to Hospital D") {
        const Hospital* res = rTree.nearest(40.1, -110.1);
        REQUIRE(res != nullptr);
        REQUIRE(res->name == "Hospital D");
    }
}

TEST_CASE("R-Tree Edge Cases and Brute Force Validation", "[rtree][brute-force]") {
    auto hospitals = getTestHospitals();
    RTree rTree;
    rTree.build(hospitals);

    std::vector<std::pair<double, double>> queryPoints = {
        {25.5, -80.5}, {30.2, -90.2}, {34.8, -99.5}, {39.9, -109.8},
        {45.2, -120.5}, {28.5, -82.5}, {31.9, -94.8}, {37.5, -104.2},
        {41.5, -111.5}, {47.0, -121.0}, {26.5, -81.5}, {29.0, -88.5},
        {36.0, -102.0}, {40.5, -113.0}, {44.5, -118.5}, {27.5, -83.5}
    };

    int caseNum = 1;
    for (const auto& q : queryPoints) {
        DYNAMIC_SECTION("Random Query Point " << caseNum++) {
            const Hospital* expected = BruteForce::findNearest(hospitals, q.first, q.second);
            const Hospital* actual = rTree.nearest(q.first, q.second);
            
            REQUIRE(actual != nullptr);
            REQUIRE(expected != nullptr);
            REQUIRE(actual->name == expected->name);
        }
    }
}