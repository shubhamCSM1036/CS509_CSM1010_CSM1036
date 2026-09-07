#include "../include/fastmap.h"

#include <cmath>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

static pair<int, int> selectPivots(
    const vector<vector<double>>& distances
)
{
    const int N = static_cast<int>(distances.size());

    int first = 0;
    int second = first;

    for (int i = 0; i < N; ++i)
    {
        if (distances[first][i] > distances[first][second])
        {
            second = i;
        }
    }

    int farthest = second;

    for (int i = 0; i < N; ++i)
    {
        if (distances[second][i] > distances[second][farthest])
        {
            farthest = i;
        }
    }

    return {second, farthest};
}

static vector<double> projectObjects(
    const vector<vector<double>>& distances,
    int pivotA,
    int pivotB
)
{
    const int N = static_cast<int>(distances.size());

    vector<double> coordinates(N, 0.0);

    const double pivotDistance =
        distances[pivotA][pivotB];

    const double epsilon = 1e-12;

    if (pivotDistance <= epsilon)
    {
        return coordinates;
    }

    const double pivotDistanceSquared =
        pivotDistance * pivotDistance;

    for (int i = 0; i < N; ++i)
    {
        const double distanceA =
            distances[pivotA][i];

        const double distanceB =
            distances[pivotB][i];

        const double numerator =
            (distanceA * distanceA) +
            pivotDistanceSquared -
            (distanceB * distanceB);

        coordinates[i] =
            numerator / (2.0 * pivotDistance);

        if (!isfinite(coordinates[i]))
        {
            coordinates[i] = 0.0;
        }
    }

    return coordinates;
}

static vector<vector<double>> deflateDistances(
    const vector<vector<double>>& distances,
    const vector<double>& coordinates
)
{
    const int N = static_cast<int>(distances.size());

    vector<vector<double>> deflated(
        N,
        vector<double>(N, 0.0)
    );

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            const double coordinateDifference =
                coordinates[i] - coordinates[j];

            double remainingSquared =
                (distances[i][j] * distances[i][j]) -
                (coordinateDifference * coordinateDifference);

            /*
             * Floating-point arithmetic can produce a very
             * small negative value even when the mathematical
             * result is zero.
             */
            if (remainingSquared < 0.0)
            {
                remainingSquared = 0.0;
            }

            deflated[i][j] =
                sqrt(remainingSquared);
        }
    }

    return deflated;
}

static bool hasRemainingDistance(
    const vector<vector<double>>& distances
)
{
    const double epsilon = 1e-12;

    for (const auto& row : distances)
    {
        for (double distance : row)
        {
            if (distance > epsilon)
            {
                return true;
            }
        }
    }

    return false;
}

FastMapResult runFastMap(
    const vector<vector<double>>& distances,
    int k
)
{
    FastMapResult result;

    const int N = static_cast<int>(distances.size());

    result.coordinates.assign(
        N,
        vector<double>(k, 0.0)
    );

    result.pivots.reserve(k);

    vector<vector<double>> currentDistances = distances;

    for (int dimension = 0; dimension < k; ++dimension)
    {
        if (!hasRemainingDistance(currentDistances))
        {
            break;
        }
        pair<int, int> pivots =
            selectPivots(currentDistances);

        result.pivots.push_back(pivots);

        vector<double> coordinates =
            projectObjects(
                currentDistances,
                pivots.first,
                pivots.second
            );

        for (double coordinate : coordinates)
        {
            if (!isfinite(coordinate))
            {
                cerr << "Error: FastMap produced a non-finite coordinate.\n";
                return result;
            }
        }

        for (int i = 0; i < N; ++i)
        {
            result.coordinates[i][dimension] =
                coordinates[i];
        }

        currentDistances =
            deflateDistances(
                currentDistances,
                coordinates
            );
    }

    return result;
}

void fastmap_fn()
{
    const string folder = "Assignment_4/tests/fastmap";

    cout << "\nChoose Input File\n";
    cout << "Available FastMap test files:\n";

    vector<string> files;

    try
    {
        for (const auto& entry : filesystem::directory_iterator(folder))
        {
            if (entry.is_regular_file() &&
                entry.path().extension() == ".txt")
            {
                files.push_back(entry.path().filename().string());
            }
        }
    }
    catch (const filesystem::filesystem_error& e)
    {
        cerr << "Error accessing FastMap test folder: "
             << e.what() << '\n';
        return;
    }

    if (files.empty())
    {
        cout << "No FastMap test files found.\n";
        return;
    }

    sort(files.begin(), files.end());

    for (size_t i = 0; i < files.size(); ++i)
    {
        cout << " " << i + 1 << ". " << files[i] << '\n';
    }

    cout << "\nEnter your choice: ";

    int choice;
    cin >> choice;

    if (!cin || choice < 1 ||
        choice > static_cast<int>(files.size()))
    {
        cout << "Invalid choice.\n";
        return;
    }

    const string filename = files[choice - 1];
    const string filepath = folder + "/" + filename;

    ifstream input(filepath);

    if (!input.is_open())
    {
        cerr << "Error: Could not open file: "
             << filepath << '\n';
        return;
    }

    int N;
    int k;

    if (!(input >> N >> k))
    {
        cerr << "Error: Invalid FastMap header. "
             << "Expected: N K\n";
        return;
    }

    if (N <= 0)
    {
        cerr << "Error: N must be greater than 0.\n";
        return;
    }

    if (k <= 0)
    {
        cerr << "Error: k must be greater than 0.\n";
        return;
    }

    if (k >= N)
    {
        cerr << "Error: k must be less than N.\n";
        return;
    }

    vector<vector<double>> distances(
        N,
        vector<double>(N)
    );

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (!(input >> distances[i][j]))
            {
                cerr << "Error: Incomplete distance matrix.\n";
                return;
            }

            if (!isfinite(distances[i][j]))
            {
                cerr << "Error: Distance values must be finite.\n";
                return;
            }

            if (distances[i][j] < 0.0)
            {
                cerr << "Error: Distance values cannot be negative.\n";
                return;
            }
        }
    }

    const double epsilon = 1e-9;

    for (int i = 0; i < N; ++i)
    {
        if (fabs(distances[i][i]) > epsilon)
        {
            cerr << "Error: Distance matrix diagonal "
                 << "must contain zero values.\n";
            return;
        }
    }

    for (int i = 0; i < N; ++i)
    {
        for (int j = i + 1; j < N; ++j)
        {
            if (fabs(distances[i][j] - distances[j][i]) > epsilon)
            {
                cerr << "Error: Distance matrix must be symmetric.\n";
                return;
            }
        }
    }

    cout << "\nInput validation successful.\n";
    cout << "Algorithm: FastMap\n";
    cout << "Input file: " << filename << '\n';
    cout << "N: " << N << '\n';
    cout << "k: " << k << '\n';

    auto start = chrono::high_resolution_clock::now();

    FastMapResult result =
        runFastMap(distances, k);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> elapsed =
        end - start;

    cout << "\nSelected pivots:\n";

    for (int dimension = 0; dimension < k; ++dimension)
    {
        cout << "Dimension " << dimension + 1
            << ": ("
            << result.pivots[dimension].first
            << ", "
            << result.pivots[dimension].second
            << ")\n";
    }

    cout << "\nFastMap coordinates:\n";

    cout << fixed << setprecision(6);

    for (int i = 0; i < N; ++i)
    {
        cout << i << ": ";

        for (int dimension = 0; dimension < k; ++dimension)
        {
            cout << result.coordinates[i][dimension];

            if (dimension + 1 < k)
            {
                cout << " ";
            }
        }

        cout << '\n';
    }

    cout << "\nExecution time: "
        << elapsed.count()
        << " ms\n";
}