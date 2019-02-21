#include "headers/utils.hpp"
#include "headers/centroids.hpp"

int main(int argc, char** argv){   
    std::vector<std::vector<float>> centers;
    
    int K_CLASSES = 2*6;
    
    int res1 = getCentroids(centers, K_CLASSES);
    int res2 = getHistograms(centers, K_CLASSES);

    return 0;
}