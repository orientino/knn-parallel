#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include "utils.h"
#include "utimer.cpp"

using namespace ff;
using namespace std;


int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << argv[0] << " [k] [nw] \n";
        return -1;
    }
    const int k = atoi(argv[1]);
    const int nw = atoi(argv[2]);

    vector<pair<double, double>> points = read_points("../data/input.data");
    vector<vector<pair<int, vector<int>>>> points_knn_local(nw);
    vector<pair<int, vector<int>>> points_knn;

    // measure parallel computation
    {
        utimer tcompute("FF-PF-Parallel");
        parallel_for_idx(0, points.size(), 1, 0, 
            [&](const long start, const long stop, const long thid) {
                points_knn_local[thid] = compute_knn(points, make_pair(start, stop), k);
            }, 
        nw);

        for (auto i=0; i<nw; ++i) 
            if (points_knn_local[i].size())
                points_knn.insert(points_knn.end(), points_knn_local[i].begin(), points_knn_local[i].end());
    }

    // print_knn(points_knn);
    // save_knn(points_knn, "../data/output.data");

    return 0;
}
