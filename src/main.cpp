#include "headers/utils.hpp"
#include "headers/centroids.hpp"
#include "headers/miscellanius.hpp"
#include "headers/SVMHandler.hpp"

int main(int argc, char** argv){   
    std::vector<std::vector<float>> centers;

    int K_CLASSES = 2*6;
    
    // Uncomment to generate centers
    int res1 = getCentroids(centers, K_CLASSES);
    
    /*
    SingleFileHandler <float> fhandler(std::string("centers_model.txt"));
    for(int i = 0; i < centers.size(); i++){
        fhandler.AppendLine(centers[i]);
    }
    fhandler.Release();
    */
   
    //Uncomment to Generate Training data"
    
    //SingleFileHandler <float> fhandler("../models/centroids/centers_model.txt");
    //fhandler.LoadFromFile(centers);
    //int res2 = getHistograms(centers, K_CLASSES);
    
   
    // Uncomment to train the data
    /*
    std::string trainingfile = "../models/training/trainingdata.txt";
    std::string labelsfile   = "../models/training/labeldata.txt";
    FileHandlerML <int> fhandler(trainingfile, labelsfile);

    std::vector<std::vector<int>> data;
    std::vector<int> y;
    std::vector<int> lbls {0, 1, 2, 3, 4, 5};
    fhandler.LoadFromFile(data, y);
    
    SVMhandler <int>svmhandler;

    svmhandler.fit(y, lbls, data);

    std::cout<<"Accuracy>> "<<svmhandler.validate(data, y)<<std::endl;
    */
    return 0;
}