#ifndef UTILS_H
#define UTILS_H

#include <utility>
#include <vector>

using namespace std;

// Given a 2D point, compute its k-nearest neighbors
vector<pair<int, vector<int>>> compute_knn(vector<int> points_idx, vector<pair<double, double>> points, int k); 

// Read file containing 2D points
vector<pair<double, double>> read_points(string path); 

// Read the results of K-NN from a file
vector<pair<int, vector<int>>> read_knn(string path);

// Save the results of K-NN into a file
void save_knn(vector<pair<int, vector<int>>> points_nn, string path);

// Prints all the points with their correspective nearest neighbors
void print_knn(vector<pair<int, vector<int>>> points_nn);

// Compute Euclidean distance between two 2D points
double compute_distance(pair<double, double> point1, pair<double, double> point2); 

#endif