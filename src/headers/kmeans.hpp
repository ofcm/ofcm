#ifndef KMEANS_HPP
#define KMEANS_HPP

#include "utils.hpp"
#include <limits>

class kmeans{
public:
    std::vector<std::vector<float>> features;
    std::vector<std::vector<float>> centers;
    std::vector<std::vector<float>> oldCenters;
    std::vector<float> classError;
    int numberClass;
    bool flag = false;
    
    kmeans(std::vector<std::vector<float>> centers,
                            int numberClass);
    void runKmeans();
    void setFeatures(std::vector<std::vector<float>>);
    std::vector<std::vector<float>> getCentroids();
    void getHistogram(std::vector<int>& result);
    void startingCenters();
    int getGoodCluster(std::vector<float>);
    float limitError = 1e-6;
};

#endif