#include "headers/utils.hpp"
#include "headers/bow.hpp"
#include "headers/miscellanius.hpp"
#include "headers/SVMHandler.hpp"
#include "getoptions.cpp"
#include "headers/kmeans.hpp"
#include "headers/plot.hpp"
#include "headers/recurrentNetwork.hpp"
// Mode
// 0: Train K-Means Centers
// 1: Generate data of training
// 2: Training SVM classifier
// 3: Prediction off-line
// 4: Prediction on-line

int main(int argc, char** argv){
    int K_CLASSES = 5;


    int mode = std::stoi(argv[1]);

    std::vector<option> train_data;
    std::vector<option> valid_data;
    std::vector<option>  test_data;

    std::string OPTION_FILE             = "../options/kth.txt";
    std::string CENTROIDS_FILE          = "../models/centroids/centroids_personperson11.txt";
    std::string TRAININGDATA_FILE       = "../models/training/traindata.txt";
    std::string TRAININGLABEL_FILE      = "../models/training/trainlabel.txt";
    std::string TESTDATA_FILE           = "../models/test/testdata.txt";
    std::string TESTLABEL_FILE          = "../models/test/testlabel.txt";

    getOptions(train_data, valid_data, test_data, OPTION_FILE);

    switch (mode)
    {
        // 0: Train K-Means Centers
        case 0: {
            std::cout<<"\n==============================================\n";
            std::cout<<"Getting Centroids ...\n";
            std::vector<std::vector<std::vector<float>>> cuboidCenters = runBOW(train_data, K_CLASSES);
            //SaveCentroidsInFile<float>(CENTROIDS_FILE, cuboidCenters);
            break;
        }
        // 1: Generate data of training
        case 1: {
            std::cout<<"\n==============================================\n";
            std::cout<<"Generating training data"<<std::endl;
            std::vector<std::vector<std::vector<float>>> cuboidCenters;
            LoadCentroidsFromFile(CENTROIDS_FILE, cuboidCenters);
            //fhandler.LoadFromFile(cuboidCenters);
            //int res2 = getHistograms(cuboidCenters, K_CLASSES);

            std::vector<std::vector<float>> histograms;
            std::vector<int> labels;
            getHistograms(train_data,K_CLASSES,cuboidCenters,histograms,labels);

            FileHandlerML <float, float> fhandlerML(TRAININGDATA_FILE, TRAININGLABEL_FILE);

            for(int i = 0; i < histograms.size(); i++)
                fhandlerML.AppendRow(histograms[i], labels[i]);
            
            fhandlerML.Release();

            break;
        }
        // 2: Training SVM classifier
        case 2: {
            std::cout<<"\n==============================================\n";
            std::cout<<"Training the SVM classifier"<<std::endl;
            std::vector<std::vector<float>> data;
            std::vector<float> y;
            std::vector<int> lbls {0, 1, 2, 3, 4, 5};
            FileHandlerML <float, float> fhandler(TRAININGDATA_FILE, TRAININGLABEL_FILE);
            fhandler.LoadFromFile(data, y);

            //SVMhandler <float> svmhandler;

            //svmhandler.fit(y, lbls, data);
            //std::cout<<"Accuracy>> "<<svmhandler.validate(data, y)<<std::endl;
            //int val = svmhandler.SaveModel("../models/svm/svm_model");
            //if(val == 0)
            //    std::cout<<"Model SVM Saved"<<std::endl;
            //else
            //    std::cout<<"Problem saving the SVM model"<<std::endl;

            break;
        }
        // 3: Prediction off-line
        case 3: {

            break;
        }
        case 4: {
            // Starting real time aplication

            // Loading Centers
            std::cout<<"Real time predictions"<<std::endl<<std::endl;
            std::cout<<"Loading Center K - means"<<std::endl;
            std::string modelcenters_file = "../models/centroids/centroids_it_168.txt";
            SingleFileHandler <float> fhandler(modelcenters_file);
            //fhandler.LoadFromFile(centers);
            std::cout<<"Model loaded ... > "<<1<<std::endl;
            // Loading SVM model
            std::cout<<"Loading SVM model"<<std::endl;
            //SVMhandler <int> svmhandler;
            //svmhandler.LoadModelFromFile("../models/svm/svm_model");

            // Initialize K - means model
            //kmeans bwords(centers, K_CLASSES);
            //bwords.setFeatures()
            break;
        }
        default:

            break;
    }
    // Uncomment to generate centers
    //int res1 = getCentroids(centers, K_CLASSES);


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
