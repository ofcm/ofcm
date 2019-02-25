#include "headers/utils.hpp"
#include "headers/clusters.hpp"
#include "headers/miscellanius.hpp"
#include "headers/SVMHandler.hpp"
#include "getoptions.cpp"
#include "headers/kmeans.hpp"
#include "headers/plot.hpp"
#include "headers/recurrentNetwork.hpp"
#include "headers/ofcm.hpp"
// Mode
// 0: Train K-Means Centers
// 1: Generate data of training
// 2: Training SVM classifier
// 3: Prediction off-line
// 4: Prediction on-line

int main(int argc, char** argv){
    int K_CLASSES = 5;
    int CUBOID_SIZE = 36;

    int mode = std::stoi(argv[1]);

    std::vector<option> train_data;
    std::vector<option> valid_data;
    std::vector<option>  test_data;

    std::string OPTION_FILE             = "../options/kth.txt";
    std::string CENTROIDS_FILE          = "../models/centroids/avrCentroids.txt";
    std::string TRAININGDATA_FILE       = "../models/training/traindata.txt";
    std::string TRAININGLABEL_FILE      = "../models/training/trainlabel.txt";
    std::string TESTDATA_FILE           = "../models/test/testdata.txt";
    std::string TESTLABEL_FILE          = "../models/test/testlabel.txt";
    std::string PATH_DATA               = "../data/";
    std::string SVM_MODEL_FILE          = "../models/svm_models/model.txt";
    getOptions(train_data, valid_data, test_data, OPTION_FILE);

    switch (mode)
    {
        // 0: Train K-Means Centers
        case 0: {
            std::cout<<"\n==============================================\n";
            std::cout<<"Getting Centroids ...\n";

            int FLAG_INIT = 0;
            std::vector<std::vector<std::vector<float>>> cuboidsCenters(35);
            getCuboidCentroids(train_data, cuboidsCenters, K_CLASSES, FLAG_INIT);
            getCuboidCentroids(valid_data, cuboidsCenters, K_CLASSES, FLAG_INIT);
            getCuboidCentroids(test_data, cuboidsCenters, K_CLASSES, FLAG_INIT);
            //SaveCentroidsInFile<float>(CENTROIDS_FILE, cuboidCenters);
            break;
        }
        // 1: Generate data of training
        case 1: {
            std::cout<<"\n==============================================\n";
            std::cout<<"Generating testing data"<<std::endl;
            std::vector<std::vector<std::vector<float>>> cuboidCenters;
            LoadCentroidsFromFile(CENTROIDS_FILE, cuboidCenters);
            //fhandler.LoadFromFile(cuboidCenters);
            //int res2 = getHistograms(cuboidCenters, K_CLASSES);

            std::vector<std::vector<float>> cuboidsClusters;
            std::vector<int> labels;
            clustering(train_data,K_CLASSES,cuboidCenters,cuboidsClusters,labels);
            /*
            FileHandlerML <float, float> fhandlerML(TRAININGDATA_FILE, TRAININGLABEL_FILE);


            for(int i = 0; i < cuboidsClusters.size(); i++)
                fhandlerML.AppendRow(cuboidsClusters[i], labels[i]);

            fhandlerML.Release();
            */
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
        // 3: train model with Train data
        //    Test with>  Test data
        case 3: {
            std::cout<<"Split data randomly:"<<std::endl;
            SVMhandler <float> svmhandler;
            std::vector<std::vector<float>> data;
            std::vector<int> y;
            std::vector<int> lbls {0, 1, 2, 3, 4, 5};

            std::vector<std::vector<float>> data_train, data_test;
            std::vector<int> y_train, y_test;

            FileHandlerML <float, int> fhandler(TRAININGDATA_FILE, TRAININGLABEL_FILE);
            FileHandlerML <float, int> fhandlerTest(TESTDATA_FILE, TESTLABEL_FILE);
            fhandler.LoadFromFile(data, y);
            fhandlerTest.LoadFromFile(data_test, y_test);

            svmhandler.shuffle_randomly_data(data, y, data_train, y_train);
            //svmhandler.split_data_train(data, y, data_train, y_train, data_test, y_test, 0.3);
            std::cout<<"Training SVM ..."<<std::endl;
            svmhandler.fit(y_train, lbls, data_train);
            std::cout<<"Making Predictions ..."<<std::endl;
            std::cout<<"Accuracy TRAIN>> "<<svmhandler.validate(data_train, y_train)<<std::endl;
            std::cout<<"Accuracy TEST>> "<<svmhandler.validate(data_test, y_test)<<std::endl;

            //svmhandler.split_randomly_data(data, y, data_, y_);

            int val = svmhandler.SaveModel(SVM_MODEL_FILE);
            if(val == 0)
                std::cout<<"Model SVM Saved"<<std::endl;
            else
                std::cout<<"Problem saving the SVM model"<<std::endl;

            break;
        }
        case 4: {
            std::string videoFile;
            // Starting real time aplication
            std::cout << "Real Time Aplication" << std::endl;
            std::cout << "####################" << std::endl;
            std::cout << "input video file name: " << std::endl;
            std::cin >> videoFile;
            cv::VideoCapture cap;
            cap.open(PATH_DATA + videoFile);
            if (!cap.isOpened())
            {
                std::cout << "Failed to open video." << std::endl;
            }
            OFCM Haralick(108,144);
            std::cout<<"Getting Haralick features ...\n";

            Haralick.get_features_realTime(cap,CUBOID_SIZE);
            std::cout<<"Getting Centroids ...\n";

            std::cout<<"Real time predictions"<<std::endl<<std::endl;
            /**here clasicador **/
            break;
        }

        case 5:
        {
            std::vector<std::vector<std::vector<float>>> cuboidCenters(35, std::vector<std::vector<float>>(K_CLASSES, std::vector<float>(12,0.0)));
            saveMeanCentroid(train_data, cuboidCenters);
        }
        // Predict off line from TESTDATA_FILE
        case 6:
        {
            std::cout<<"Using model to predict\n";
            SVMhandler <float> svmhandler;
            if(svmhandler.LoadModelFromFile(SVM_MODEL_FILE)){
                std::cout<<"Model Loaded!"<<std::endl;
            }

            std::vector<std::vector<float>> data_test;
            std::vector<int> y_test;

            FileHandlerML <float, int> fhandlerTest(TESTDATA_FILE, TESTLABEL_FILE);
            fhandlerTest.LoadFromFile(data_test, y_test);
            std::cout<<"Accuracy TEST>> "<<svmhandler.validate(data_test, y_test)<<std::endl;
            break;
        }
        // Concatenate Test data and Train data
        // Concatenate and Split 30% to test cross validation
        case 7:
        {
            std::cout<<"Split data randomly:"<<std::endl;
            SVMhandler <float> svmhandler;
            std::vector<std::vector<float>> data;
            std::vector<int> y;
            std::vector<int> lbls {0, 1, 2, 3, 4, 5};

            std::vector<std::vector<float>> data_train, data_test;
            std::vector<int> y_train, y_test;

            FileHandlerML <float, int> fhandler(TRAININGDATA_FILE, TRAININGLABEL_FILE);
            FileHandlerML <float, int> fhandlerTest(TESTDATA_FILE, TESTLABEL_FILE);
            fhandler.LoadFromFile(data, y);
            fhandlerTest.LoadFromFile(data_test, y_test);

            for(unsigned long k = 0; k < data_test.size(); k++){
                data.push_back(data_test[k]);
                y.push_back(y_test[k]);
            }
            std::cout<<"Shape data merged> \t["<<data.size()<<", "<<data[0].size()<<"]"<<std::endl;
            std::cout<<"Labels shape> \t\t["<<y.size()<<"]"<<std::endl;

            data_test.clear();
            y_test.clear();
            //svmhandler.shuffle_randomly_data(data, y, data_train, y_train);
            svmhandler.split_data_train(data, y, data_train, y_train, data_test, y_test, 0.3);
            std::cout<<"Training SVM ..."<<std::endl;
            svmhandler.fit(y_train, lbls, data_train);
            std::cout<<"Making Predictions ..."<<std::endl;
            std::cout<<"Accuracy TRAIN>> "<<svmhandler.validate(data_train, y_train)<<std::endl;
            std::cout<<"Accuracy TEST>> "<<svmhandler.validate(data_test, y_test)<<std::endl;

            //svmhandler.split_randomly_data(data, y, data_, y_);

            int val = svmhandler.SaveModel(SVM_MODEL_FILE);
            if(val == 0)
                std::cout<<"Model SVM Saved"<<std::endl;
            else
                std::cout<<"Problem saving the SVM model"<<std::endl;

            break;
        }
        case 8:
        {
            std::cout<<"\n==============================================\n";
            std::cout<<"Generating testing data"<<std::endl;
            std::vector<std::vector<std::vector<float>>> cuboidCenters;
            LoadCentroidsFromFile(CENTROIDS_FILE, cuboidCenters);
            //fhandler.LoadFromFile(cuboidCenters);
            //int res2 = getHistograms(cuboidCenters, K_CLASSES);

            std::vector<std::vector<float>> cuboidsClusters;
            std::vector<int> labels;
            realTime(train_data,K_CLASSES,cuboidCenters,cuboidsClusters,labels);
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
