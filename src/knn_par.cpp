#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <thread>
#include <algorithm>
#include <mutex>
#include "utils.h"
#include "utimer.cpp"

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

    // compute the local knn
    vector<pair<int, vector<int>>> points_knn_local = compute_knn(points, make_pair(start, stop), k); 

    // add the local result to the global result
    globallock.lock();
    points_knn.insert(points_knn.end(), points_knn_local.begin(), points_knn_local.end());
    globallock.unlock();
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << argv[0] << " [k] [nw] \n";
        return -1;
    }
    const int k = atoi(argv[1]);
    const int nw = atoi(argv[2]);

    vector<pair<double, double>> points = read_points("../data/input.data");
    vector<pair<int, vector<int>>> points_knn;
    vector<thread*> tids(nw);
    mutex globallock;

    // measure parallel computation
    {
        utimer tcompute("Parallel");
        for (auto i=0; i<nw; ++i)
            tids[i] = new thread(thread_body, i, points, ref(points_knn), nw, k, ref(globallock));
        for(int i=0; i<nw; ++i)
            tids[i]->join(); 
        sort(points_knn.begin(), points_knn.end());
    }

    // print_knn(points_knn);
    // save_knn(points_knn, "../data/output.data");

    return 0;
}
