#include "BruteForce.h"
#include "KD-Tree.h"
#include "Point.h"
#include "RTree.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

//one query point that's the block group's location
struct Query{
    double latitude;
    double longitude;
    
};

//splits the CSV line into its fields, comma inside double quotes stays a part of the field
std::vector<std::string> split_csv(const std::string& line){
    std::vector<std::string> row_val(1); // an empty field to fill up
    bool inside = false; 
    //checks if inside the quotes
    
    for (std::size_t i = 0; i< line.size(); ++i){
     const char ch = line[i];
        if (ch == '"'){
            if (inside && i+1 <line.size() && line[i+1] == '"'){
                row_val.back() += '"';
                ++i; 
            } else{
                inside = !inside; //entering or leaving the quotes
            }
        } else if (ch == ',' && !inside) {
            row_val.emplace_back();
            
        } else{
            row_val.back() +=ch;
        }
    }
    return row_val;
   
}

std::vector<std::string> read_header(std::ifstream& file, const std::string&path ) {

    std::string line;
    if (!std::getline(file, line)) { //reads column names and keeps it going to next row
        throw std::runtime_error("Could not read " + path);
    }
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    return split_csv(line); //returns the columns as vector of strings
}

std::size_t column(const std::vector<std::string>& header,
                   const std::string& name) {
                    //finds the index of a column
    const auto position = std::find(header.begin(), header.end(), name);
    if (position == header.end()) {
        throw std::runtime_error("Missing CSV column " + name);
    }
    return static_cast<std::size_t>(position - header.begin());
}

//checking for states outside the continental US (Alaska, Hawaii, territories)
bool outside_continental_state(const std::string& state) {
    return state == "AK" || state == "HI" || state == "PR" || state == "GU" ||
           state == "VI" || state == "AS" || state == "MP";
}

//checking for states outside continental US with codes
bool outside_continental_fips(const std::string& fips) {
    return fips == "02" || fips == "15" || fips == "60" || fips == "66" ||
           fips == "69" || fips == "72" || fips == "78";
}

std::vector<Hospital> load_hospitals(const std::string& path) {
    std:: ifstream file(path);
    const auto header = read_header(file, path);
    const auto name = column(header, "Official Name");
    const auto code = column(header, "Feature Code");
    const auto state = column(header, "State Abbreviation");
    const auto x = column(header, "x");
    const auto y = column(header, "y");

    constexpr double earth_radius = 6378137.0;
    constexpr double degrees_per_radian = 180.0 / 3.14159265358979323846;

    std::vector<Hospital> hospitals;
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty() || line == "\r") {
        continue;   
    }   
        const auto row = split_csv(line);
        if (row.at(code) != "80012") {
            continue; //skips rows with different codes
        }
        if (outside_continental_state(row.at(state))) {
            continue; 
        }
        //changing the x and y strings to numbers
        const double lat = std::atan(std::sinh(std::stod(row.at(y)) / earth_radius))
                         * degrees_per_radian;
                         
        const double lon = std::stod(row.at(x)) / earth_radius * degrees_per_radian;
        hospitals.push_back({row.at(name), lat, lon});
    }

    return hospitals;
}


std::vector<Query> load_queries(const std::string& path){
    std::ifstream file(path);
    const auto header = read_header(file, path);
    const auto fips = column(header, "State Federal Information Processing Series (FIPS) codes");
    const auto lat = column(header, "Internal Point Latitude");
    const auto lon = column(header, "Internal Point Longitude");
    std::vector<Query> queries;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line == "\r") {
            continue; // skip blank lines
        }
        const auto row = split_csv(line);
        if (outside_continental_fips(row.at(fips))) {
            continue;
        }
        queries.push_back({std::stod(row.at(lat)), std::stod(row.at(lon))});
    }

    return queries;
}

int main() {
    const auto hospitals = load_hospitals("Structures_Medical_Emergency_Response_v1_3359501503281473884.csv");
    if (hospitals.empty()) {                       
        std::cout << "No hospitals loaded.\n";
        return 1;
    }
    const auto queries = load_queries("USA_Block_Groups_v1_-2119205518973722401.csv");
    std::cout <<"Hospitals loaded: " <<hospitals.size() << "\n";
    std::cout << "Block groups loaded: " << queries.size() << "\n";
    KD_Tree kd;
    kd.build(hospitals);
    RTree rt;
    rt.build(hospitals);
    
    // an example: Gainesville, Fl
    const double lat = 29.65, lon = -82.32;
        const Hospital* a = kd.nearest(lat, lon);
        const Hospital* b = rt.nearest(lat, lon);
        const Hospital* c = BruteForce::findNearest(hospitals, lat, lon);

    std::cout << "k-d tree:    " << a->name << "\n";
std::cout << "R-tree:      " << b->name << "\n";
std::cout << "Brute force: " << c->name << "\n";

return 0;
}