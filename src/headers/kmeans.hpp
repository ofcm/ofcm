#ifndef KMEANS_HPP
#define KMEANS_HPP

#include "utils.hpp"
#include <limits>

class kmeans{
public:
    std::vector<std::vector<float>> features;
    std::vector<std::vector<float>> * cluster;
    std::vector<std::vector<float>> centers;
    std::vector<std::vector<float>> oldCenters;
    std::vector<float> classError;
    int numberClass;
    bool flag;
    float minError = std::numeric_limits<double>::infinity();
    
    kmeans(std::vector<std::vector<float>>,std::vector<std::vector<float>>,
                        int);
    std::vector<std::vector<float>> runKmeans(std::vector<int>&);
    void startingCenters();
    int getGoodCluster(std::vector<float>);
    float limitError = 0.0001;
};

#endif