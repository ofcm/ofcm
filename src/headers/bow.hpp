#ifndef CENTROIDS_HPP
#define CENTROIDS_HPP

#include "utils.hpp"
#include "option.hpp"

void addString(std::string& src, std::string s, int maxSize);

int getCentroids(std::vector<option> database, std::vector<std::vector<std::vector<float>>>& centers, int K_CLASES);
int getHistograms(std::vector<std::vector<float>> centers, int K_CLASES);

#endif