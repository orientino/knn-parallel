#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>

#include "utils.h"

// Given a 2D point, compute its k-nearest neighbors
vector<pair<int, vector<int>>> compute_knn(vector<int> points_idx, vector<pair<double, double>> points, int k) {
    vector<pair<int, vector<int>>> points_nn;
    int n = points.size() - 1;

    for (auto idx: points_idx) {
        // compute and sort by the distance
        vector<pair<double, int>> neighbors;
        for (auto i=0; i<points.size(); i++) {
            if (idx != i) {
                double dist = compute_distance(points[idx], points[i]);
                neighbors.push_back(make_pair(dist, i));
            }
        }
        sort(neighbors.begin(), neighbors.end());

        // save the k-nearest neighbors
        vector<int> knn;
        for (auto i=0; i<k; i++) 
            knn.push_back(neighbors[i].second);

        // add element
        points_nn.push_back(make_pair(idx, knn));
    } 
    return points_nn;
}

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

// Read the results of K-NN from a file
vector<pair<int, vector<int>>> read_knn(string path) {
    vector<pair<int, vector<int>>> points_nn; 

    ifstream fin(path);
    string line;
    while (getline(fin, line, '\n')) {
        int idx;
        vector<int> nn;
        stringstream ss(line);

        ss >> idx;
        ss.ignore();
        int tmp_idx;
        while (ss >> tmp_idx) {
            nn.push_back(tmp_idx); 
            ss.ignore();
        }
        points_nn.push_back(make_pair(idx, nn));
    }

    return points_nn;
}

// Save the results of K-NN into a file
void save_knn(vector<pair<int, vector<int>>> points_nn, string path) {
    ofstream fout(path);
    for (auto nn: points_nn) {
        stringstream ss;
        ss << nn.first << ":";
        for (auto point: nn.second)
            ss << point << ",";
        ss << '\n';

        fout << ss.str();
    }
    fout.close();
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

// Compute Euclidean distance between two 2D points
double compute_distance(pair<double, double> point1, pair<double, double> point2) {
    double x = point1.first - point2.first;
    double y = point1.second - point2.second;
    double dist;

    dist = sqrt(pow(x, 2) + pow(y, 2));
    return dist;
}