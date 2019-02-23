#ifndef CENTROIDS_HPP
#define CENTROIDS_HPP

#include "utils.hpp"
#include "option.hpp"

void addString(std::string& src, std::string s, int maxSize);

std::vector<std::vector<std::vector<float>>> runBOW(std::vector<option> database, 
                                                    int mClusters, 
                                                    bool HISTOGRAM, 
                                                    std::vector<std::vector<float>>& result,
                                                    std::vector<float>& labels);
//int getHistograms(std::vector<std::vector<float>> centers, int K_CLASES);

#endif