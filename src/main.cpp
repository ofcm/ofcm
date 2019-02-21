#include "headers/utils.hpp"
#include "headers/centroids.hpp"
#include "headers/miscellanius.hpp"
#include "headers/SVMHandler.hpp"

// Mode
// 0: Train K-Means Centers
// 1: Generate data of training
// 2: Training SVM classifier
// 3: Prediction mode

int mode = 0;
int main(int argc, char** argv){   
    int K_CLASSES = 2*6;
    std::vector<std::vector<float>> centers;
    switch (mode)
    {
        case 0: {
            int res1 = getCentroids(centers, K_CLASSES);
            break;
        }
        case 1: {
            std::string modelcenters_file = "../models/centroids/centers_model.txt";
            SingleFileHandler <float> fhandler(modelcenters_file);
            fhandler.LoadFromFile(centers);
            int res2 = getHistograms(centers, K_CLASSES);
            break;
        }
        case 2: {
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
            break;
        }
        case 3: {
            break;
        }
        default:

            break;
    }
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
    
    //std::string trainingfile = "../models/training/trainingdata.txt";
    //std::string labelsfile   = "../models/training/labeldata.txt";
    //FileHandlerML <int> fhandler(trainingfile, labelsfile);

    //std::vector<std::vector<int>> data;
    //std::vector<int> y;
    //std::vector<int> lbls {0, 1, 2, 3, 4, 5};
    //fhandler.LoadFromFile(data, y);
    //
    //SVMhandler <int>svmhandler;

    //svmhandler.fit(y, lbls, data);

    //std::cout<<"Accuracy>> "<<svmhandler.validate(data, y)<<std::endl;
    
    return 0;
}