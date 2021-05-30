#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <mutex>

#include "utils.h"

using namespace std;

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

void thread_body(
                    int threadno, 
                    vector<pair<double, double>> points, 
                    vector<pair<int, vector<int>>> &points_nn, 
                    int nw, 
                    int k,
                    mutex &globallock
                ) {
    // compute the part of work
    auto n = points.size();
    auto delta = n / nw;
    auto start = threadno * delta;
    auto stop  = threadno == (nw-1) ? n : (threadno+1) * delta;

    vector<int> idx;
    for (auto i=start; i<stop; i++)
        idx.push_back(i);

    // compute the local knn
    vector<pair<int, vector<int>>> points_nn_local = compute_knn(idx, points, k); 

    // add the local result to the global result
    globallock.lock();
    points_nn.insert(points_nn.end(), points_nn_local.begin(), points_nn_local.end());
    globallock.unlock();
}

int main(int argc, char *argv[]) {
    int nw = atoi(argv[1]);
    int k = atoi(argv[2]);

    vector<pair<double, double>> points = read_points("./data.csv");
    vector<int> idx;
    for (auto i=0; i<points.size(); i++)
        idx.push_back(i);

    // sequential computation
    auto start = chrono::high_resolution_clock::now();

    vector<pair<int, vector<int>>> points_nn_seq = compute_knn(idx, points, k);

    auto elapsed = chrono::high_resolution_clock::now() - start;
    auto usec = chrono::duration_cast<chrono::microseconds>(elapsed).count();
    cout << "Sequential time (usec): " << usec << endl;

    // parallel computation
    mutex globallock;
    start = chrono::high_resolution_clock::now();
    vector<pair<int, vector<int>>> points_nn_par;

    vector<thread*> tids(nw);
    for (auto i=0; i<nw; i++)
        tids[i] = new thread(thread_body, i, points, ref(points_nn_par), nw, k, ref(globallock));
    for(int i=0; i<nw; i++)
        tids[i]->join(); 
    sort(points_nn_par.begin(), points_nn_par.end());

    elapsed = chrono::high_resolution_clock::now() - start;
    usec = chrono::duration_cast<chrono::microseconds>(elapsed).count();
    cout << "Parallel time (usec): " << usec << endl;

    // check results from sequential and parallel computations
    // if (points_nn_seq == points_nn_par)
    //     cout << "Correct results!" << endl;

    return 0;
}
