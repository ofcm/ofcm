#ifndef CENTROIDS_HPP
#define CENTROIDS_HPP

#include "utils.hpp"
#include "option.hpp"

void addString(std::string& src, std::string s, int maxSize);

std::vector<std::vector<std::vector<float>>> runBOW(std::vector<option> database, 
                                                    int mClusters);
void getHistograms( std::vector<option> database, 
                    int mClusters,
                    std::vector<std::vector<std::vector<float>>> cuboidsCenters,
                    std::vector<std::vector<float>>& histograms,
                    std::vector<int>& labels);

void saveMeanCentroid(std::vector<option> database, std::vector<std::vector<std::vector<float>>> meanCuboidsCenters);

#endif