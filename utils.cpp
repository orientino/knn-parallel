#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

#include "utils.h"

// Read file containing 2D points
vector<pair<double, double>> read_points(string path) {
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

    return points;
}

// Compute Euclidean distance between two 2D points
double compute_distance(pair<double, double> point1, pair<double, double> point2) {
    double x = point1.first - point2.first;
    double y = point1.second - point2.second;
    double dist;

    dist = sqrt(pow(x, 2) + pow(y, 2));
    return dist;
}

// Prints all the points with their correspective nearest neighbors
void print_knn(vector<pair<int, vector<int>>> points_nn) {
    for (auto nn: points_nn) {
        cout << nn.first << ": "; 
        for (auto point: nn.second)
            cout << point << " ";
        cout << endl;
    }
}

