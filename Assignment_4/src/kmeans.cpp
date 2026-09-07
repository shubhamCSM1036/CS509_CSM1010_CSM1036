#include "../include/kmeans.h"

#include <cmath>
#include <limits>
#include <algorithm>
#include <bits/stdc++.h>
#include <fstream>
#include <chrono>

using namespace std;


static double squaredDistance(
    const vector<double>& point,
    const vector<double>& centroid
) {
    double distance = 0.0;

    for (size_t d = 0; d < point.size(); ++d) {
        double diff = point[d] - centroid[d];
        distance += diff * diff;
    }

    return distance;
}


KMeansResult runKMeans(
    const vector<vector<double>>& points,
    int K,
    long long maxIterations,
    double tolerance
) {
    KMeansResult result;

    const int N = static_cast<int>(points.size());
    const int D = static_cast<int>(points[0].size());


    result.assignment.assign(N, -1);

    result.centroids.resize(K);

    for (int k = 0; k < K; ++k) {
        result.centroids[k] = points[k];
    }

    result.wcss = 0.0;
    result.iterations = 0;
    result.converged = false;


    for (long long iteration = 0;
         iteration < maxIterations;
         ++iteration) {

        ++result.iterations;


        bool assignmentChanged = false;

        for (int i = 0; i < N; ++i) {

            int nearestCluster = -1;

            double minimumDistance =
                numeric_limits<double>::max();

            for (int k = 0; k < K; ++k) {

                double distance = squaredDistance(
                    points[i],
                    result.centroids[k]
                );

                if (distance < minimumDistance) {
                    minimumDistance = distance;
                    nearestCluster = k;
                }
            }

            if (result.assignment[i] != nearestCluster) {
                assignmentChanged = true;
                result.assignment[i] = nearestCluster;
            }
        }



        vector<vector<double>> newCentroids(
            K,
            vector<double>(D, 0.0)
        );

        vector<int> clusterSizes(K, 0);

        for (int i = 0; i < N; ++i) {

            int cluster = result.assignment[i];

            ++clusterSizes[cluster];

            for (int d = 0; d < D; ++d) {
                newCentroids[cluster][d] += points[i][d];
            }
        }

        for (int k = 0; k < K; ++k) {

            if (clusterSizes[k] == 0) {

                newCentroids[k] = result.centroids[k];

            } else {

                for (int d = 0; d < D; ++d) {
                    newCentroids[k][d] /=
                        static_cast<double>(clusterSizes[k]);
                }
            }
        }


        double maximumShift = 0.0;

        for (int k = 0; k < K; ++k) {

            double shiftSquared = 0.0;

            for (int d = 0; d < D; ++d) {

                double difference =
                    newCentroids[k][d]
                    - result.centroids[k][d];

                shiftSquared += difference * difference;
            }

            double shift = sqrt(shiftSquared);

            maximumShift =
                max(maximumShift, shift);
        }

        result.centroids = newCentroids;


        if (!assignmentChanged ||
            maximumShift <= tolerance) {

            result.converged = true;
            break;
        }
    }


    result.wcss = 0.0;

    for (int i = 0; i < N; ++i) {

        int cluster = result.assignment[i];

        result.wcss += squaredDistance(
            points[i],
            result.centroids[cluster]
        );
    }

    return result;
}


void kmeans_fn() {

    string folder = "Assignment_4/tests/kmeans";
    vector<string> files;

    for (const auto& entry : filesystem::directory_iterator(folder))
    {
        if (entry.is_regular_file())
        {
            files.push_back(entry.path().string());
        }
    }

    if (files.empty())
    {
        cout << "No files found in " << folder << "\n";
        return;
    }

    cout << "Kmeans algorithm test files:\n\n";

    for (size_t i = 0; i < files.size(); i++)
    {
        cout << i + 1 << ". "
             << filesystem::path(files[i]).filename().string()
             << "\n";
    }

    int choice;

    cout << "\nEnter serial number: ";
    cin >> choice;

    if (choice < 1 || static_cast<size_t>(choice) > files.size())
    {
        cout << "Invalid serial number.\n";
        return;
    }

    string selectedFile = files[choice - 1];

    ifstream fin(selectedFile);

    if (!fin) {
        cout << "Error: could not open input file '"
             << selectedFile << "'\n";
    }

   

    int N, D, K;

    if (!(fin >> N >> D >> K)) {
        cout << "Error: missing or invalid 'N D K' header in '"
             << selectedFile << "'\n";
    }

    if (N <= 0) {
        cout << "Error: N must be greater than 0.\n";
    }

    if (D <= 0) {
        cout << "Error: D must be greater than 0.\n";
    }

    if (K <= 0) {
        cout << "Error: K must be greater than 0.\n";
    }

    if (K > N) {
        cout << "Error: K (" << K
             << ") cannot exceed N (" << N << ").\n";
    }

  

    vector<vector<double>> points(N, vector<double>(D));

    for (int i = 0; i < N; ++i) {

        for (int d = 0; d < D; ++d) {

            if (!(fin >> points[i][d])) {
                cout << "Error: truncated point data at row "
                     << i << " in '"
                     << selectedFile << "'\n";
            }
        }
    }

    

    string tag;
    long long maxIterations;

    if (!(fin >> tag >> maxIterations)) {
        cout << "Error: missing 'MAX_ITERATIONS n' in '"
             << selectedFile << "'\n";
    }

    if (tag != "MAX_ITERATIONS") {
        cout << "Error: expected 'MAX_ITERATIONS' but found '"
             << tag << "'.\n";
    }

    if (maxIterations <= 0) {
        cout << "Error: MAX_ITERATIONS must be greater than 0.\n";
    }

   

    double tolerance;

    if (!(fin >> tag >> tolerance)) {
        cout << "Error: missing 'TOLERANCE epsilon' in '"
             << selectedFile << "'\n";
    }

    if (tag != "TOLERANCE") {
        cout << "Error: expected 'TOLERANCE' but found '"
             << tag << "'.\n";
    }

    if (tolerance <= 0.0) {
        cout << "Error: TOLERANCE must be greater than 0.\n";
    }


    auto start = chrono::high_resolution_clock::now();

    KMeansResult result = runKMeans(
        points,
        K,
        maxIterations,
        tolerance
    );

    auto stop = chrono::high_resolution_clock::now();

    double executionTime = chrono::duration<double, milli>(stop - start).count();



    cout << "Algorithm: K-Means Clustering\n";
    cout << "K: " << K << "\n";


    cout << "Point assignments:\n";

    for (int i = 0; i < N; ++i) {
        cout << i << " "
             << result.assignment[i]
             << "\n";
    }


    cout << "Final centroids:\n";

    cout << fixed << setprecision(6);

    for (int k = 0; k < K; ++k) {

        cout << k << ":";

        for (int d = 0; d < D; ++d) {
            cout << " "
                 << result.centroids[k][d];
        }

        cout << "\n";
    }


    cout << "WCSS: " << result.wcss << "\n";


    cout << "Iterations: " << result.iterations << "\n";
    cout << "Converged: " << (result.converged ? "true" : "false") << "\n";
    cout << "Execution time: " << executionTime << " ms\n";

}