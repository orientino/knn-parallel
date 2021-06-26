#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <ff/ff.hpp>
#include <ff/farm.hpp>
#include <chrono>
#include "utils.h"

using namespace ff;
using namespace std;
using ull = unsigned long long;

struct Task_t {
    vector<int> points_idx;
    vector<pair<double, double>> points;
    vector<pair<int, vector<int>>> result;
    int k;

    Task_t(vector<int> points_idx, vector<pair<double, double>> points, int k): 
        points_idx(points_idx),
        points(points),
        k(k) {}
};

struct Emitter: ff_monode_t<Task_t> {
    vector<pair<double, double>> points;
    vector<pair<int, vector<int>>> points_knn;
    int k;

    Emitter(vector<pair<double, double>> points, int k): 
        points(points), 
        k(k) {}

    Task_t *svc(Task_t *in) {
        // start of the emitter
        if (in == nullptr) {
            // distribute the workload to the slaves
            const int nw = get_num_outchannels();
            const auto size = points.size() / (nw+1);
            ull start = 0;
            ull stop = size;

            for (auto i=0; i<nw; ++i) {
                vector<int> points_idx;
                for (auto i=start; i<stop; ++i) 
                    points_idx.push_back(i);

                Task_t *task = new Task_t(points_idx, points, k);
                ff_send_out(task, i);

                if (i < nw-1) {
                    start = stop;
                    stop = start + size;
                }
            }
            broadcast_task(EOS);

            // compute the last partition locally and add to the final result
            start = stop;
            stop = points.size();

            vector<int> points_idx;
            for (auto i=start; i<stop; ++i) 
                points_idx.push_back(i);

            vector<pair<int, vector<int>>> points_knn_local;
            points_knn_local = compute_knn(points_idx, points, k);
            points_knn.insert(points_knn.end(), points_knn_local.begin(), points_knn_local.end());

            return GO_ON;
        }
 
        // processing the feedback channels from the slaves
        // add the slaves' output to the final result
        auto &slave_result = in->result;
        if (slave_result.size())
            points_knn.insert(points_knn.end(), slave_result.begin(), slave_result.end());
        delete in;

        return GO_ON;
    }

    void svc_end() {
        sort(points_knn.begin(), points_knn.end());
    }
};

struct Worker: ff_node_t<Task_t> {
    Task_t *svc(Task_t *in) {
        in->result = compute_knn(in->points_idx, in->points, in->k);
        return in;
    }
};

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "use: " << argv[0] << " nw k \n";
        return -1;
    }

    const int nw = atoi(argv[1]);
    const int k = atoi(argv[2]);

    if (nw < 2) {
        cerr << "use the ./knn_seq for sequential computation. \n";
        return -1;
    }

    vector<pair<double, double>> points = read_points("../data/input.data");

    // parallel computation using a master-slave skeleton
    ff_Farm<> farm([&]() {
        vector<unique_ptr<ff_node>> W;
        for (auto i=0; i<(nw-1); ++i)
            W.push_back(make_unique<Worker>());
        return W;
    }());

    // modify the farm skeleton into master-slave
    Emitter E(points, k);
    farm.add_emitter(E);
    farm.remove_collector();
    farm.wrap_around();

    if (farm.run_and_wait_end() < 0) {
        error("running farm\n");
        return -1;
    }
    cout << "Fastflow time (msec): " << farm.ffTime() << endl;

    // print_knn(E.points_knn);
    // save_knn(E.points_knn, "../data/output.data");

    return 0;
}
