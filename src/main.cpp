#include "headers/utils.hpp"
#include "headers/centroids.hpp"
#include "headers/miscellanius.hpp"

int main(int argc, char** argv){   
    std::vector<std::vector<float>> centers;
    int K_CLASSES = 2*6;
    int res = getCentroids(centers, K_CLASSES);

    SingleFileHandler <float> fhandler(std::string("centers_model.txt"));
    for(int i = 0; i < centers.size(); i++){
        fhandler.AppendLine(centers[i]);
    }
    fhandler.Release();
    return 0;
}