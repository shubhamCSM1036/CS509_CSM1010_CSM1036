#ifndef KMEANS_H
#define KMEANS_H

#include <vector>

using namespace std;

struct KMeansResult {
    vector<int> assignment;
    vector<vector<double>> centroids;

    double wcss;
    long long iterations;
    bool converged;
};

KMeansResult runKMeans(
    const vector<vector<double>>& points,
    int K,
    long long maxIterations,
    double tolerance
);

void kmeans_fn();

#endif