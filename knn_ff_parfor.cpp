#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

#include "utils.h"

using namespace ff;
using namespace std;
using ull = unsigned long long;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "use: " << argv[0] << " nw k \n";
        return -1;
    }

    const int nw = atoi(argv[1]);
    const int k = atoi(argv[2]);

    if (nw < 2) {
        cerr << "use the ./knn_seq for sequential computation. \n";
        return -1;
    }

    vector<pair<double, double>> points = read_points("./input.data");
    vector<pair<int, vector<int>>> points_knn;
    vector<vector<pair<int, vector<int>>>> points_knn_local(nw);

    // parallel computation using fastflow's parallel for
    ffTime(START_TIME);
    auto chunk = 0;
    parallel_for_idx(0, points.size(), 1, chunk, 
        [&](const long start, const long stop, const long thid) {
            vector<int> points_idx;
            for (auto i=start; i<stop; ++i)
                points_idx.push_back(i);

            points_knn_local[thid] = compute_knn(points_idx, points, k);
        }, 
    nw);

    for (auto i=0; i<nw; ++i) 
        if (points_knn_local[i].size())
            points_knn.insert(points_knn.end(), points_knn_local[i].begin(), points_knn_local[i].end());
    ffTime(STOP_TIME);
    cout << "Fastflow-ParallelFor time (msec): " << ffTime(GET_TIME) << endl;

    return 0;
}
