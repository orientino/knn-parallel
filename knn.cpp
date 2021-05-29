#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <chrono>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <sstream>

using namespace std;

// Read file
vector<pair<double, double>> read_points(string path, int verbose=0) {
    vector<pair<double, double>> points;

    ifstream fin(path);
    string line;
    double n1, n2;
    while (getline(fin, line, '\n')) {
        stringstream ss(line);
        ss >> n1;
        ss.ignore();
        ss >> n2;
        points.push_back(make_pair(n1, n2));
    }
    fin.close();

    // printing
    if (verbose > 0)
        for (auto point: points) 
            cout << point.first << "," << point.second << endl;

    return points;
}

// Compute Euclidean distance between two 2D point
double compute_distance(pair<double, double> point1, pair<double, double> point2) {
    double x = point1.first - point2.first;
	double y = point1.second - point2.second;
	double dist;

	dist = sqrt(pow(x, 2) + pow(y, 2));
	return dist;
}

// Given a 2D point, compute its k-nearest neighbors
pair<int, vector<int>> compute_nn(int point, vector<pair<double, double>> points, int k) {
    pair<int, vector<int>> point_nn;
    point_nn.first = point;

    // compute and sort by the distance
    int n = points.size() - 1;
    vector<pair<double, int>> neighbors;
    for (auto i=0; i<points.size(); i++) {
        if (point != i) {
            double dist = compute_distance(points[point], points[i]);
            neighbors.push_back(make_pair(dist, i));
        }
    }
    sort(neighbors.begin(), neighbors.end());

    // save the k-nearest neighbors
    vector<int> knn;
    for (auto i=0; i<k; i++) 
        knn.push_back(neighbors[i].second);
    point_nn.second = knn;
    return point_nn;
}

int main(int argc, char *argv[]) {
    int nw = atoi(argv[1]);
    int k = atoi(argv[2]);

    vector<pair<double, double>> points = read_points("./data.csv");

    // sequential computation
    auto start = chrono::high_resolution_clock::now();

    vector<pair<int, vector<int>>> points_nn;
    for (auto i=0; i<points.size(); i++) 
        points_nn.push_back(compute_nn(i, points, k));

    auto elapsed = chrono::high_resolution_clock::now() - start;
    auto usec = chrono::duration_cast<chrono::microseconds>(elapsed).count();
    cout << "Sequential time (usec): " << usec << endl;

    // parallel computation
    // auto start = chrono::high_resolution_clock::now();

    // vector<pair<int, vector<int>>> points_nn;
    // for (auto i=0; i<points.size(); i++) 
    //     points_nn.push_back(compute_nn(i, points, k));

    // auto elapsed = chrono::high_resolution_clock::now() - start;
    // auto usec = chrono::duration_cast<chrono::microseconds>(elapsed).count();
    // cout << "Sequential time (usec): " << usec << endl;

    for (auto nn: points_nn) {
        cout << nn.first << ": "; 
        for (auto point: nn.second)
            cout << point << " ";
        cout << endl;
    }

    return 0;
}
