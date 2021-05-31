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

void thread_body(
                    int threadno, 
                    vector<pair<double, double>> points, 
                    vector<pair<int, vector<int>>> &points_knn, 
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
    for (auto i=start; i<stop; ++i)
        idx.push_back(i);

    // compute the local knn
    vector<pair<int, vector<int>>> points_knn_local = compute_knn(idx, points, k); 

    // add the local result to the global result
    globallock.lock();
    points_knn.insert(points_knn.end(), points_knn_local.begin(), points_knn_local.end());
    globallock.unlock();
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "use: " << argv[0] << " nw k \n";
        return -1;
    }

    const int nw = atoi(argv[1]);
    const int k = atoi(argv[2]);

    vector<pair<double, double>> points = read_points("./input.data");

    // parallel computation
    auto start = chrono::high_resolution_clock::now();

    vector<pair<int, vector<int>>> points_knn;
    vector<thread*> tids(nw);
    mutex globallock;

    for (auto i=0; i<nw; ++i)
        tids[i] = new thread(thread_body, i, points, ref(points_knn), nw, k, ref(globallock));
    for(int i=0; i<nw; ++i)
        tids[i]->join(); 
    sort(points_knn.begin(), points_knn.end());

    auto elapsed = chrono::high_resolution_clock::now() - start;
    auto msec = chrono::duration_cast<chrono::milliseconds>(elapsed).count();
    cout << "Parallel time (msec): " << msec << endl;

    return 0;
}
