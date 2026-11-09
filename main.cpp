#include "RTree.h"
#include "Point.h"
#include <iostream>
#include <random>
#include <vector>
#include <string>

struct Query{
    double latitude;
    double longitude;
};

std::vector<std::string> split_csv(const std::string& line){
    std::vector<std::string> row_val(1);
    bool inside = false;
    //getting the value of each row in csv file
    for (std::size_t i = 0; i< line.size(); ++i){
     const char ch = line[i];
        if (ch == '"'){
            if (inside && i+1 <line.size() && line[i+1] == '"'){
                row_val.back() += '"';
                ++i;
            } else{
                inside = !inside;
            }
        } else if (ch == ',' && !inside) {
            row_val.emplace_back();

        } else{
            row_val.back() +=ch;
        }
    }
    return row_val;
   
}
