//#include "headers/utils.hpp"
#include "headers/ofcm.hpp"
#include "headers/kmeans.hpp"
#include "getoptions.cpp"
#include "headers/miscellanius.hpp"
#include "headers/centroids.hpp"


std::vector<std::string> itos       = {"boxing","handclapping","handwaving","jogging","running","walking"};
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

//std::string PERSON = "/person";
//std::string SUB    = "_";
std::string END    = "_uncomp.avi";



int getCentroids(std::vector<std::vector<float>>& centers, int K_CLASES){   

    //std::vector<int> trainingPerson    = {11, 12, 13, 14, 15, 16, 17, 18};
    std::vector<std::string> filenames(6);
    std::vector<std::vector<std::vector<float>>> personActionfeatures(6);
    cv::VideoCapture cap;
    bool INIT          = true;

    std::string PATH_DATA               = "../data/";
    kmeans BOW(centers, K_CLASES);

    std::vector<option> train_data;
    std::vector<option> valid_data;
    std::vector<option>  test_data;

    
    std::string FILENAME = "../data/kth.txt";

    getOptions(train_data, valid_data, test_data, FILENAME);
    /*
    for(int i = 0; i < train_data.size();i++)
        std::cout << train_data[i].person << std::endl;
    */
    for (int itrain = 0; itrain < train_data.size(); itrain+=24)
    {
        for (int ipaf = 0; ipaf < personActionfeatures.size(); ipaf++)
        {
            personActionfeatures[ipaf].clear();
        }        
        for (int idata = 0; idata < 24; idata++)
        {
            int ifile = idata/6;
            std::string PERSON = train_data[itrain + idata].person;
            std::string ACTION = train_data[itrain + idata].action;
            std::string TYPE   = train_data[itrain + idata].d;

            std::string filename_temp = "";
            for (int i = 0; i < ACTION.size(); i++)
                filename_temp += ACTION[i];
            filename_temp +=  "/";
            for (int i = PERSON.size()-8; i < PERSON.size(); i++)
                filename_temp += PERSON[i];
            filename_temp +=  "_";
            for (int i = 0; i < ACTION.size(); i++)
                filename_temp += ACTION[i];
            filename_temp +=  "_";
            for (int i = 0; i < TYPE.size(); i++)
                filename_temp += TYPE[i];
            filename_temp +=  END;

            std::string videlname = PATH_DATA + filename_temp;
            std::cout << videlname << std::endl;
            cap.open(videlname);

            if (!cap.isOpened())
            {
                std::cout << "Failed to open camera." << std::endl;
                return -1;
            }

            for (int iv = 0;  iv < train_data[itrain + idata].sequence.size(); iv+=2)
            {
                std::pair<int,int> input_sequence(train_data[itrain + idata].sequence[0], train_data[itrain + idata].sequence[1]);

                std::vector<std::vector<float>> res = OFCM(cap, input_sequence);

                for(int ir = 0; ir < res.size(); ir++)
                {
                    personActionfeatures[ifile].push_back(res[ir]);
                }
            }
        }
        std::vector<std::vector<float>> toKmeans;

        int ikm = 0;

        bool ALL = true;
        while (true)
        {
            ALL = true;
            toKmeans.clear();
            for (int ifile = 0; ifile < filenames.size(); ifile++)
            {
                if (ikm >= personActionfeatures[ifile].size())
                {
                    ALL *= true;
                    continue;
                }
                ALL *= false;

                
                for(int itf = ikm; itf < ikm + 280; itf++)
                {
                    toKmeans.push_back(personActionfeatures[ifile][itf]);
                }
            }

            if (ALL == true)
                break;

            std::vector<int> result; 

            BOW.setFeatures(toKmeans);
            if (INIT == true)
            {
                BOW.startingCenters();
                INIT = false;
            }          
            BOW.runKmeans();       
            centers = BOW.getCentroids();
            
            std::cout << "\ncenters size = " << centers.size() << std::endl;
            std::cout << "====================" << std::endl;
            for (int k = 0; k < K_CLASES; k++)
            {
                for (int icen = 0; icen < centers[k].size(); icen++)
                {
                    std::cout << centers[k][icen] << " ";
                }
                std::cout << std::endl;
            }         
            
            //BOW.getHistogram(result);                      
            //std::cout << "\nClass = "<< itos[ifile] << " => ";
            //fhandler.AppendLine(result, ifile);

            for(int ires = 0; ires < result.size(); ires++)
                std::cout << result[ires] << " ";

            
            ikm += 280;
        }
        /*
        for(int isce = 0; isce < scenarios.size(); isce++)
        {
            std::cout << "sceneario = " << scenarios[isce] << std::endl;
            for (int ipaf = 0; ipaf < personActionfeatures.size(); ipaf++)
            {
                personActionfeatures[ipaf].clear();
            }
            for(int ifile = 0; ifile < filenames.size(); ifile++)
            {
                filenames[ifile] = itos[ifile].c_str() + train_data[itrain].person + "_" + itos[ifile] + "_" + scenarios[isce] + END;


                cap.open(PATH_DATA+filenames[ifile]);

                if (!cap.isOpened())
                {
                    std::cout << "Failed to open camera." << std::endl;
                    return -1;
                }
                std::pair<int,int> sequence(1, 230);

                std::vector<std::vector<float>> res = OFCM(cap, sequence);

                for(int ir = 0; ir < res.size(); ir++)
                {
                    personActionfeatures[ifile].push_back(res[ir]);
                }
                /*
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
                toKmeans.clear();
                for (int ifile = 0; ifile < filenames.size(); ifile++)
                {
                    if (ikm >= personActionfeatures[ifile].size())
                    {
                        ALL *= true;
                        continue;
                    }
                    ALL *= false;

                    
                    for(int itf = ikm; itf < ikm + 280; itf++)
                    {
                        toKmeans.push_back(personActionfeatures[ifile][itf]);
                    }
                }

                if (ALL == true)
                    break;

                std::vector<int> result; 

                BOW.setFeatures(toKmeans);
                if (INIT == true)
                {
                    BOW.startingCenters();
                    INIT = false;
                }          
                BOW.runKmeans();       
                centers = BOW.getCentroids();
                
                std::cout << "\ncenters size = " << centers.size() << std::endl;
                std::cout << "====================" << std::endl;
                for (int k = 0; k < K_CLASES; k++)
                {
                    for (int icen = 0; icen < centers[k].size(); icen++)
                    {
                        std::cout << centers[k][icen] << " ";
                    }
                    std::cout << std::endl;
                }         
                
                //BOW.getHistogram(result);                      
                //std::cout << "\nClass = "<< itos[ifile] << " => ";
                //fhandler.AppendLine(result, ifile);

                for(int ires = 0; ires < result.size(); ires++)
                    std::cout << result[ires] << " ";

                
                ikm += 280;
            }

        }
        */
    }
    /*
    fhandler.Release();
    std::vector<std::vector<int>> data;
    std::vector<int> et;
    fhandler.LoadFromFile(data, et);
    */
    return 0;
}