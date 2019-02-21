#include "headers/utils.hpp"
#include "headers/centroids.hpp"
#include "getoptions.cpp"

int main(int argc, char** argv){   
    std::vector<std::vector<float>> centers;
    int K_CLASSES = 4*6;
    //int res = getCentroids(centers, K_CLASSES);

    std::vector<option> train_data;
    std::vector<option> valid_data;
    std::vector<option>  test_data;

    std::string FILENAME = "../data/kth.txt";
    getOptions(train_data, valid_data, test_data, FILENAME);

    for (int i = 0; i < train_data.size(); i++)
    {
        std::cout << train_data[i].person << std::endl;
        std::cout << train_data[i].action << std::endl;
        std::cout << train_data[i].d << std::endl;

        for (int iv = 0; iv < train_data[i].sequence.size(); iv++)
            std::cout << train_data[i].sequence[iv] << " "; 
        std::cout << "\n" << std::endl;
    }
    return 0;
}