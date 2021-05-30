#ifndef UTILS_H
#define UTILS_H

#include <utility>
#include <vector>

using namespace std;

// Read file containing 2D points
vector<pair<double, double>> read_points(string path); 

// Compute Euclidean distance between two 2D points
double compute_distance(pair<double, double> point1, pair<double, double> point2); 

// Prints all the points with their correspective nearest neighbors
void print_knn(vector<pair<int, vector<int>>> points_nn);

#endif