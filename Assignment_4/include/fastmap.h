#ifndef FASTMAP_H
#define FASTMAP_H

#include <utility>
#include <vector>

struct FastMapResult
{
    std::vector<std::vector<double>> coordinates;
    std::vector<std::pair<int, int>> pivots;
};

FastMapResult runFastMap(
    const std::vector<std::vector<double>>& distances,
    int k
);

void fastmap_fn();

#endif