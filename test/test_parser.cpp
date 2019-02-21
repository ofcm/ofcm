#include "headers/miscellanius.hpp"

int main(){

    std::vector<std::vector<float>> data{{0.8f , 0.8f}, {0.7f , 0.7f},
                                        {0.9f , 0.8f}, {-0.8f , -0.9f},
                                        {-0.8f , -0.7f}, {-0.7f , -0.7f}};

    SingleFileHandler <float> fhandler(std::string("outexample.txt"));
    for(int i = 0; i < data.size(); i++){
        fhandler.AppendLine(data[i]);
    }
    fhandler.Release();
    std::vector<std::vector<float>> d;
    fhandler.LoadFromFile(d);
    for(int i = 0; i < d.size(); i++){
        for(int j = 0; j < d[0].size(); j++){
            std::cout<<d[i][j]<<"\t";
        }
        std::cout<<std::endl;
    }
    return 0;
}