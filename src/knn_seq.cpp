#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <chrono>
#include <algorithm>
#include "utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "use: " << argv[0] << " k \n";
        return -1;
    }

    const int k = atoi(argv[1]);

    vector<pair<double, double>> points = read_points("../data/input.data");

    // sequential computation
    auto start = chrono::high_resolution_clock::now();

    vector<int> idx;
    for (auto i=0; i<points.size(); ++i)
        idx.push_back(i);

    vector<pair<int, vector<int>>> points_knn = compute_knn(idx, points, k);

    auto elapsed = chrono::high_resolution_clock::now() - start;
    auto msec = chrono::duration_cast<chrono::milliseconds>(elapsed).count();
    cout << "Sequential time (msec): " << msec << endl;

    // print_knn(points_knn);
    save_knn(points_knn, "../data/output.data");

    return 0;
}
