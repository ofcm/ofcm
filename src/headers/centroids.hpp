#ifndef CENTROIDS_HPP
#define CENTROIDS_HPP

#include "utils.hpp"

int getCentroids(std::vector<std::vector<float>>& centers, int K_CLASES);
int getHistograms(std::vector<std::vector<float>> centers, int K_CLASES);

#endif