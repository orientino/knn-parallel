#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>

using namespace std;

// Read file
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

    return points;
}

// Given a 2D point, compute its k-nearest neighbors
unordered_map<int, vector<int>> compute_pairs(int point, vector<pair<double, double>> points, int k) {
    unordered_map<int, vector<int>> pairs;
    return pairs;
}

// Compute distance between two 2D point
double compute_distance() {
    return 0;
}

int main(int argc, char *argv[]) {
    int nw = atoi(argv[1]);

    vector<pair<double, double>> points = read_points("./data.csv");
    for (auto i=0; i<points.size(); i++) {
        cout << points[i].first << "," << points[i].second << endl;
    }

    return 0;
}
