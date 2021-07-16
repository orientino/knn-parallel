#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <queue>
#include "utils.h"


vector<pair<int, vector<int>>> compute_knn(vector<pair<double, double>> points, pair<int, int> range, int k) {
    vector<pair<int, vector<int>>> points_nn;
    int n = points.size() - 1;

    for (int i=range.first; i<range.second; i++) {
        // compute and use the max-heap data structure
        priority_queue<pair<double, int>> neighbors;
        for (auto j=0; j<points.size(); j++) {
            if (i != j) {
                double dist = compute_distance(points[i], points[j]);
                if (neighbors.size() < k)
                    neighbors.push(make_pair(dist, j));
                else if (neighbors.top().first > dist) {
                    neighbors.pop();
                    neighbors.push(make_pair(dist, j));
                }
            }
        }

        // save the k-nearest neighbors
        // the order of the neighbors is descendent
        vector<int> knn;
        for (auto j=0; j<k; j++) {
            knn.push_back(neighbors.top().second);
            neighbors.pop();
        } 

        // add element
        points_nn.push_back(make_pair(i, knn));
    } 
    return points_nn;
}


// vector<pair<int, vector<int>>> compute_knn(vector<pair<double, double>> points, pair<int, int> range, int k) {
//     vector<pair<int, vector<int>>> points_nn;
//     int n = points.size() - 1;

//     for (int i=range.first; i<range.second; i++) {
//         // compute and sort by the distance
//         vector<pair<double, int>> neighbors;
//         for (auto j=0; j<points.size(); j++) {
//             if (i != j) {
//                 double dist = compute_distance(points[i], points[j]);
//                 neighbors.push_back(make_pair(dist, j));
//             }
//         }
//         sort(neighbors.begin(), neighbors.end());

//         // save the k-nearest neighbors
//         vector<int> knn;
//         for (auto j=0; j<k; j++) 
//             knn.push_back(neighbors[j].second);

//         // add element
//         points_nn.push_back(make_pair(i, knn));
//     } 
//     return points_nn;
// }


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


void print_knn(vector<pair<int, vector<int>>> points_nn) {
    for (auto nn: points_nn) {
        cout << nn.first << ": "; 
        for (auto point: nn.second)
            cout << point << " ";
        cout << endl;
    }
}


double compute_distance(pair<double, double> point1, pair<double, double> point2) {
    double x = point1.first - point2.first;
    double y = point1.second - point2.second;
    return sqrt(pow(x, 2) + pow(y, 2));
}