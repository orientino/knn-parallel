#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <ff/ff.hpp>
#include <ff/farm.hpp>
#include "utils.h"
#include "utimer.cpp"

using namespace ff;
using namespace std;


struct Task_t {
    vector<pair<double, double>> points;
    pair<int, int> range;
    vector<pair<int, vector<int>>> result;
    int k;

    Task_t(vector<pair<double, double>> points, pair<int, int> range, int k): 
        points(points),
        range(range),
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
            int start = 0;
            int stop = size;

            for (auto i=0; i<nw; ++i) {
                Task_t *task = new Task_t(points, make_pair(start, stop), k);
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

            vector<pair<int, vector<int>>> points_knn_local;
            points_knn_local = compute_knn(points, make_pair(start, stop), k);
            points_knn.insert(points_knn.end(), points_knn_local.begin(), points_knn_local.end());

            return GO_ON;
        }
 
        // add the slaves' output to the final result
        auto &slave_result = in->result;
        if (slave_result.size())
            points_knn.insert(points_knn.end(), slave_result.begin(), slave_result.end());
        delete in;

        return GO_ON;
    }

    // sort the vector at the end
    void svc_end() {
        sort(points_knn.begin(), points_knn.end());
    }
};


struct Worker: ff_node_t<Task_t> {
    Task_t *svc(Task_t *in) {
        in->result = compute_knn(in->points, in->range, in->k);
        return in;
    }
};


int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << argv[0] << " [k] [nw] \n";
        return -1;
    }
    const int k = atoi(argv[1]);
    const int nw = atoi(argv[2]);

    vector<pair<double, double>> points = read_points("../data/input.data");

    // master-slave skeleton
    ff_Farm<> farm([&]() {
        vector<unique_ptr<ff_node>> W;
        for (auto i=0; i<(nw-1); ++i)
            W.push_back(make_unique<Worker>());
        return W;
    }());
    Emitter E(points, k);
    farm.add_emitter(E);
    farm.remove_collector();
    farm.wrap_around();

    // measure parallel computation
    {
        utimer tcompute("FF-MS-Parallel");
        if (farm.run_and_wait_end() < 0) {
            error("running farm\n");
            return -1;
        }
    }

    // print_knn(E.points_knn);
    // save_knn(E.points_knn, "../data/output.data");

    return 0;
}
