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
        //USGS feature code 80012 identifies hospitals and medical centers.
        if (row.at(code) != "80012") {
            continue; //skips rows with different codes
        }
        if (outside_continental_state(row.at(state))) {
            continue; 
        }
        //Convert Web Mercator x/y in meters to longitude/latitude in degrees.
        const double latitude = std::atan(std::sinh(std::stod(row.at(y)) / earth_radius))
                         * degrees_per_radian;
                         
        const double longitude = std::stod(row.at(x)) / earth_radius * degrees_per_radian;
        hospitals.push_back({row.at(name), latitude, longitude});
    }

    return hospitals;
}


std::vector<Query> load_queries(const std::string& path){
    std::ifstream file(path);
    const auto header = read_header(file, path);
    const auto fips = column(header, "State Federal Information Processing Series (FIPS) codes");
    const auto latitude = column(header, "Internal Point Latitude");
    const auto longitude = column(header, "Internal Point Longitude");
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
        queries.push_back({std::stod(row.at(latitude)), std::stod(row.at(longitude))});
    }

    return queries;
}
//squared distance in degrees (the trees use the same)
double dist_sq(const Hospital* h, const Query& q) {
    double d_lat = h->latitude - q.latitude, d_lon = h->longitude - q.longitude;
    return d_lat * d_lat + d_lon * d_lon;
}

//real-world distance in miles, only used for printing
double miles(double lat1, double lon1, double lat2, double lon2) {
    const double r = 3.14159265358979323846 / 180.0;
    double a = std::pow(std::sin((lat2 - lat1) * r / 2), 2) +
               std::cos(lat1 * r) * std::cos(lat2 * r) * std::pow(std::sin((lon2 - lon1) * r / 2), 2);
    return 3958.8 * 2 * std::asin(std::sqrt(a));
}

//returns the time it took
template <typename F>
double time_ms(F f) {
    auto start = std::chrono::steady_clock::now();
    f();
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
}

int main() {

    try{
        const auto hospitals = load_hospitals("Structures_Medical_Emergency_Response_v1_3359501503281473884.csv");
    if (hospitals.empty()) {                       
        std::cout << "No hospitals loaded.\n";
        return 1;
    }
    const auto queries = load_queries("USA_Block_Groups_v1_-2119205518973722401.csv");
    std::cout <<"Hospitals loaded: " <<hospitals.size() << "\n";
    std::cout << "Block groups loaded: " << queries.size() << "\n";
    KD_Tree kd;
    
    RTree rt;
    
    double kd_build = time_ms([&] { kd.build(hospitals); });
    double rt_build = time_ms([&] { rt.build(hospitals); });
    
    std::string choice;
        while (std::cout << "\n1. Find nearest hospital\n2. Run benchmark\n3. Exit\nChoice: "
               && std::cin >> choice && choice != "3") {
            if (choice == "1") {
                Query q;
                std::cout << "Enter latitude and longitude: ";
                if (!(std::cin >> q.latitude >> q.longitude)) {
                    std::cout << "Please enter two numbers, e.g. 29.65 -82.32\n";
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    continue;
                }
                const Hospital* a = nullptr;
                const Hospital* b = nullptr;
                double kd_ms = time_ms([&] { a = kd.nearest(q.latitude, q.longitude); });
                double rt_ms = time_ms([&] { b = rt.nearest(q.latitude, q.longitude); });
                std::cout << "Nearest hospital: " << a->name << "\n"
                          << "Distance: " << miles(q.latitude, q.longitude, a->latitude, a->longitude) << " miles\n"
                          << "k-d tree search: " << kd_ms << " ms\n"
                          << "R-tree search:   " << rt_ms << " ms\n";
            } else if (choice == "2") {
                const std::size_t n = queries.size();
                std::vector<const Hospital*> kd_ans(n), rt_ans(n), bf_ans(n);
                std::cout << "Running " << n << " block groups (brute force can take a minute)...\n";

                //time for each method
                double kd_ms = time_ms([&] { for (std::size_t i = 0; i < n; ++i) kd_ans[i] = kd.nearest(queries[i].latitude, queries[i].longitude); });
                double rt_ms = time_ms([&] { for (std::size_t i = 0; i < n; ++i) rt_ans[i] = rt.nearest(queries[i].latitude, queries[i].longitude); });
                double bf_ms = time_ms([&] { for (std::size_t i = 0; i < n; ++i) bf_ans[i] = BruteForce::findNearest(hospitals, queries[i].latitude, queries[i].longitude); });

                //comparing distances not the names
                std::size_t kd_wrong = 0, rt_wrong = 0;
                for (std::size_t i = 0; i < n; ++i) {
                    kd_wrong += dist_sq(kd_ans[i], queries[i]) != dist_sq(bf_ans[i], queries[i]);
                    rt_wrong += dist_sq(rt_ans[i], queries[i]) != dist_sq(bf_ans[i], queries[i]);
                }

                std::cout << "Build time (ms):   k-d " << kd_build << " | R-tree " << rt_build << "\n"
                          << "All queries (ms):  k-d " << kd_ms << " | R-tree " << rt_ms << " | brute force " << bf_ms << "\n"
                          << "Avg / query (ms):  k-d " << kd_ms / n << " | R-tree " << rt_ms / n << " | brute force " << bf_ms / n << "\n"
                          << "Matches brute force: " << (kd_wrong == 0 && rt_wrong == 0 ? "YES" : "NO") << "\n";
            } else {
                std::cout << "Enter 1, 2, or 3.\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n"; //for missing files
        return 1;
    }
    return 0;
}