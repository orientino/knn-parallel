#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include "utils.h"
#include "utimer.cpp"

using namespace std;


int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << argv[0] << " [k] \n";
        return -1;
    }
    const int k = atoi(argv[1]);
    vector<pair<double, double>> points;
    vector<pair<int, vector<int>>> points_knn;

    // measure reading time
    {
        // utimer tread("Reading");
        points = read_points("../data/input.data");
    }

    // measure sequential time
    {
        utimer tcompute("Sequential");
        points_knn = compute_knn(points, make_pair(0, points.size()), k);
    }

    // measure writing time
    {
        // utimer twrite("Writing");
        // print_knn(points_knn);
        save_knn(points_knn, "../data/output.data");
    }

    return 0;
}
