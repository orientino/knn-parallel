#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <fstream>

using namespace std;

// Generate n pair of points between the interval [-range, +range]
vector<pair<double, double>> generate_pairs(int n, int range, int verbose=0) {
    vector<pair<double, double>> points;

    for (auto i=0; i<n; i++) {
        double n1 = static_cast<double> (rand() / static_cast<double> (RAND_MAX / range)) * 2 - range;
        double n2 = static_cast<double> (rand() / static_cast<double> (RAND_MAX / range)) * 2 - range;
        points.push_back(make_pair(n1, n2));

        if (verbose > 0) 
            cout << n1 << "," << n2 << endl;
    }

    return points;
}

int main(int argc, char *argv[]) {
    int seed = atoi(argv[1]);
    int n = atoi(argv[2]);
    int range = atoi(argv[3]);

    if (argc != 4) {
        cout << "use parameters [seed, n, range]." << endl;
        return -1;
    } 

    srand(seed);
    vector<pair<double, double>> points = generate_pairs(n, range, 1);

    // save the generated points into a file
    ofstream fout("./data.csv");
    for (auto const& x : points)
        fout << x.first << "," << x.second << endl; 
    fout.close();

    return 0;
}