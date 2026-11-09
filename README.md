<H1> Hospital Helper </h1>
Hospital Helper finds the closest hospital to any given point in the United States using latitude and longitude coordinates, implemented via R Tree, KD Tree, and brute force algorithms.

<h2> Team Tree Triage </h2>

- Maya Venkatraman (mdvenkatraman)
- Xavier Ruiz-Pagan (xruizzz)
- Divya Singh (Divya0822)

<h2> Features </h2>

1. **Find nearest hospital:** enter a latitude and longitude (for example `29.65 -82.32`) and the program prints the nearest hospital, the distance in miles, and how long the k-d tree and R-tree each took.
2. **Benchmark:** runs all 238,194 continental US block groups through the k-d tree, the R-tree, and brute force, then prints build times, query times, and whether both trees matched brute force.

<h2> Data </h2>

- `Structures_Medical_Emergency_Response_v1_3359501503281473884.csv`: USGS hospital locations (6,401 in the continental US)
- `USA_Block_Groups_v1_-2119205518973722401.csv`: U.S. Census block groups, used as the query points (238,194)

Only the continental US (48 states + DC) is used.

<h2> How to Build </h2>

You need a C++17 compiler. From the project folder:

```bash
g++ -std=c++17 -O2 -I"Header Files" -I"Header Files/KD Tree Header Files" -I"Header Files/R Tree Header Files" main.cpp KD-Tree.cpp RTree.cpp -o HospitalHelper
```

<h2> How to Run </h2>

Run it from the project folder, since it opens the CSV files by name:

```bash
./HospitalHelper
```

On Windows the file is `HospitalHelper.exe`. The benchmark can take up to a minute because brute force checks every hospital for every block group.

<h2> About Each File </h2>

- `main.cpp`: loads the data, builds both trees, runs the menu and the benchmark
- `KD-Tree.cpp`, `Header Files/KD Tree Header Files/KD-Tree.h`: k-d tree
- `RTree.cpp`, `Header Files/R Tree Header Files/RTree.h`, `Rect.h`: R-tree
- `Header Files/BruteForce.h`: brute-force nearest search, used to check the trees
- `Header Files/Point.h`: the `Hospital` struct shared by everything
- `Parser.cpp`, `Header Files/Parser.h`: CSV parser class
- `tests.cpp`: Catch2 unit tests for both trees
