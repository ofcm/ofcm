#include "headers/utils.hpp"
#include "headers/centroids.hpp"

int main(int argc, char** argv){   
    std::vector<std::vector<float>> centers;
    int K_CLASSES = 4*6;
    int res = getCentroids(centers, K_CLASSES);

    return 0;
}