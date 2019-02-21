#include "headers/utils.hpp"
#include "headers/ofcm.hpp"
#include "headers/kmeans.hpp"
#include "getOptions.cpp"
#include "headers/miscellanius.hpp"
#include "headers/SVMHandler.hpp"

std::string PATH_DATA               = "../data/";
std::vector<std::string> itos       = {"walking","jogging","running","boxing","handwaving","handclapping"};
std::vector<std::string> nperson    = { "01","02","03","04","05","06","07","08","09","10",
                                        "11","12","13","14","15","16","17","18","19","20",
                                        "21","22","23","24","25"};
std::vector<std::string> scenarios  = {"d1","d2","d3","d4"};

std::string filename0;
std::string filename1;
std::string filename2;
std::string filename3;
std::string filename4;
std::string filename5;

std::string PERSON = "/person";
std::string SUB    = "_";
std::string END    = "_uncomp.avi";

bool INIT          = true;

int main(int argc, char** argv){   
    std::vector<int> trainingPerson    = {11,12,13};
    std::vector<std::string> filenames(6);
    std::vector<std::vector<std::vector<float>>> personActionfeatures(6);
    cv::VideoCapture cap;

    std::vector<std::vector<float>> centers;

    kmeans BOW(centers, 30);
    FileHandler <int> fhandler("traindata.txt", "testdata.txt");
    for (int itrain = 0; itrain < 1; itrain++)
    {
        for(int isce = 0; isce < scenarios.size(); isce++)
        {
            for (int ipaf = 0; ipaf < personActionfeatures.size(); ipaf++)
            {
                personActionfeatures[ipaf].clear();
            }
            for(int ifile = 0; ifile < filenames.size(); ifile++)
            {
                filenames[ifile] = itos[ifile].c_str() + PERSON + std::to_string(trainingPerson[itrain]) + "_" + itos[ifile] + "_" + scenarios[isce] + END;
                cap.open(PATH_DATA+filenames[ifile]);

                if (!cap.isOpened())
                {
                    std::cout << "Failed to open camera." << std::endl;
                    return -1;
                }
                std::pair<int,int> sequence(1, 20);
                std::vector<std::vector<float>> res = OFCM(cap, sequence);

                for(int ir = 0; ir < res.size(); ir++)
                {
                    personActionfeatures[ifile].push_back(res[ir]);
                }
                
                std::cout << "personActionfeatures size 0: " << personActionfeatures[0].size() << std::endl;
                std::cout << "personActionfeatures size 1: " << personActionfeatures[1].size() << std::endl;
                std::cout << "personActionfeatures size 2: " << personActionfeatures[2].size() << std::endl;
                std::cout << "personActionfeatures size 3: " << personActionfeatures[3].size() << std::endl;
                std::cout << "personActionfeatures size 4: " << personActionfeatures[4].size() << std::endl;
                std::cout << "personActionfeatures size 5: " << personActionfeatures[5].size() << std::endl;
                
            }

            std::vector<std::vector<float>> toKmeans;

            int ikm = 0;

            bool ALL = true;
            while (true)
            {
                ALL = true;
                for (int ifile = 0; ifile < filenames.size(); ifile++)
                {
                    if (ikm >= personActionfeatures[ifile].size())
                    {
                        ALL *= true;
                        continue;
                    }
                    ALL *= false;

                    toKmeans.clear();
                    for(int itf = ikm; itf < ikm + 280; itf++)
                    {
                        toKmeans.push_back(personActionfeatures[ifile][itf]);
                    }
                    std::vector<int> result; 
                    std::cout << "toKmeans size = " << toKmeans.size() << std::endl;
                    //std::cout << "runing kmeans " << std::endl;
                    
                    BOW.setFeatures(toKmeans);
                    if (INIT == true)
                    {
                        BOW.startingCenters();
                        INIT = false;
                    }          
                    BOW.runKmeans(result);
                    fhandler.AppendLine(result, ifile);

                    for(int ires = 0; ires < result.size(); ires++)
                        std::cout << result[ires] << " ";
                    std::cout << "\n=========" << std::endl;
                    
                }

                if (ALL == true)
                    break;
                
                ikm += 280;
            }

        }
    }
    fhandler.Release();
    std::vector<std::vector<int>> data;
    std::vector<int> y_;
    fhandler.LoadFromFile(data, y_);

    /*
     * Creating SVM model
     */
    // Since historygram data is integer
    // By default it is initialized with RBF kernell
    SVMhandler <int> svmhandler;
    std::vector<int> labelsV{0, 1, 2, 3, 4, 5};
    bool val = svmhandler.fit(y_, labelsV, data, nullptr);

    float acc = svmhandler.validate(data, y_);
    std::cout<<"Accuracy:>> "<<acc<<std::endl;
    return 0;
}