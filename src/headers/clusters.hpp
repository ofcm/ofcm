#ifndef CENTROIDS_HPP
#define CENTROIDS_HPP

#include "utils.hpp"
#include "option.hpp"

void addString(std::string& src, std::string s, int maxSize);

std::vector<std::vector<std::vector<float>>> getCuboidCentroids(std::vector<option> database, 
                                                    int mClusters);
void clustering( std::vector<option> database, 

                    int mClusters,
                    std::vector<std::vector<std::vector<float>>> cuboidsCenters,
                    std::vector<std::vector<float>>& histograms,
                    std::vector<int>& labels);
void getCentroid(std::vector<std::vector<std::vector<float>>> personActionfeatures,
                 std::vector<std::vector<std::vector<float>>>& cuboidsCenters,
                 int classes,
                 int mclusters,
                 int setRandomCenter);

void saveMeanCentroid(std::vector<option> database, std::vector<std::vector<std::vector<float>>> meanCuboidsCenters);

#endif
