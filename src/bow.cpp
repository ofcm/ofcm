//#include "headers/utils.hpp"

#include "headers/ofcm.hpp"
#include "headers/kmeans.hpp"
#include "headers/option.hpp"
#include "headers/miscellanius.hpp"
#include "headers/bow.hpp"

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

std::string END    = "_uncomp.avi";


void addString(std::string& src, std::string s, int maxSize)
{
    for (int i = s.size()-maxSize; i < s.size(); i++)
        src += s[i]; 
}

int getCentroids(std::vector<option> database, std::vector<std::vector<float>>& centers, int K_CLASES){   
    std::vector<std::string> filenames(6);
    std::vector<std::vector<std::vector<float>>> personActionfeatures(6);
    cv::VideoCapture cap;
    bool INIT          = true;

    std::string PATH_DATA               = "../data/";
    kmeans BOW(centers, K_CLASES);
    
    std::string MODEL_CENTROIDS_PATH = "../models/centroids/";

    
    
    std::cout << "data size : " << database.size() << std::endl;

    for (int itrain = 0; itrain < database.size(); itrain+=24)
    {
        for (int ipaf = 0; ipaf < personActionfeatures.size(); ipaf++)
        {
            personActionfeatures[ipaf].clear();
        }        
        for (int idata = 0; idata < 24; idata++)
        {
            int ifile = idata/4;

            std::string PERSON = database[itrain + idata].person;
            std::string ACTION = database[itrain + idata].action;
            std::string TYPE   = database[itrain + idata].d;

            std::string videofile = "";
            addString(videofile, ACTION, ACTION.size());
            videofile += "/";
            addString(videofile, PERSON, 8);
            videofile +=  "_";
            addString(videofile, ACTION, ACTION.size());
            videofile +=  "_";
            addString(videofile, TYPE  , TYPE.size());
            videofile +=  END;

            std::cout << PATH_DATA + videofile << std::endl;
            cap.open(PATH_DATA + videofile);

            if (!cap.isOpened())
            {
                std::cout << "Failed to open camera." << std::endl;
                return -1;
            }

            //for (int iv = 0;  iv < train_data[itrain + idata].sequence.size(); iv+=2)
            for (int iv = 0;  iv < 2; iv+=2)
            {
                std::pair<int,int> input_sequence(database[itrain + idata].sequence[iv], database[itrain + idata].sequence[iv + 1]);
                OFCM Haralick(108,144);
                std::vector<std::vector<std::vector<float>>> res = Haralick.get_features(cap, input_sequence);
                //for(int ir = 0; ir < res.size(); ir++)
               // {
                //    personActionfeatures[ifile].push_back(res[ir]);
                //}
            }
        }
        /*
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

                int itf_limit = (ikm + 35 < personActionfeatures[ifile].size()) ? ikm + 35: personActionfeatures[ifile].size(); 
                for(int itf = ikm; itf < itf_limit; itf++)
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

            
            for (int ifile = 0; ifile < filenames.size(); ifile++)
            {
                toKmeans.clear();
                if (ikm >= personActionfeatures[ifile].size())
                {
                    ALL *= true;
                    continue;
                }
                ALL *= false;

                int itf_limit = (ikm + 35 < personActionfeatures[ifile].size()) ? ikm + 35: personActionfeatures[ifile].size(); 
                for(int itf = ikm; itf < itf_limit; itf++)
                {
                    toKmeans.push_back(personActionfeatures[ifile][itf]);
                }
                BOW.getHistogram(result);                      
                std::cout << "\nClass = "<< itos[ifile] << " => ";
                //fhandler.AppendLine(result, ifile);

                for(int ires = 0; ires < result.size(); ires++)
                    std::cout << result[ires] << " ";
                std::cout << std::endl;
            }
            
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
            
            //BW.getHistogram(result);                      
            //std::cout << "\nClass = "<< itos[ifile] << " => ";
            //fhandler.AppendLine(result, ifile);

            //for(int ires = 0; ires < result.size(); ires++)
            //    std::cout << result[ires] << " ";

            
            ikm += 35;
        }
        // ** Save model
        std::string fnme = "centroids_it_" +std::to_string(itrain) + ".txt";
        SingleFileHandler <float> fhandler(MODEL_CENTROIDS_PATH + fnme);
        for(int ii = 0; ii < centers.size(); ii++)
            fhandler.AppendLine(centers[ii]);
        fhandler.Release();
        */
    }

    return 0;
}


int getHistograms(std::vector<std::vector<float>> centers, int K_CLASES){   

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
    std::string MODEL_TRAINING_PATH = "../models/test/";

    // ** Class Writer historigram
    FileHandlerML <int> fhandlerML(MODEL_TRAINING_PATH + "testdata.txt",MODEL_TRAINING_PATH + "labeldata_test.txt");

    //getOptions(train_data, valid_data, test_data, FILENAME);
    /*
    for(int i = 0; i < train_data.size();i++)
        std::cout << train_data[i].person << std::endl;
    */
    for (int itrain = 0; itrain < test_data.size(); itrain+=24)
    //for (int itrain = 0; itrain < 24; itrain+=24)
    {
        for (int ipaf = 0; ipaf < personActionfeatures.size(); ipaf++)
        {
            personActionfeatures[ipaf].clear();
        }        
        for (int idata = 0; idata < 24; idata++)
        {
            int ifile = idata/4;

            std::string PERSON = test_data[itrain + idata].person;
            std::string ACTION = test_data[itrain + idata].action;
            std::string TYPE   = test_data[itrain + idata].d;

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

            for (int iv = 0;  iv < test_data[itrain + idata].sequence.size(); iv+=2)
            //for (int iv = 0;  iv < 2; iv+=2)
            {
                std::pair<int,int> input_sequence(test_data[itrain + idata].sequence[iv], test_data[itrain + idata].sequence[iv + 1]);

                OFCM Haralick(108,144);
                std::vector<std::vector<std::vector<float>>> res = Haralick.get_features(cap, input_sequence);

                //for(int ir = 0; ir < res.size(); ir++)
                //{
                //    personActionfeatures[ifile].push_back(res[ir]);
                //}
            }
        }
        /*
        std::vector<std::vector<float>> toKmeans;

        int ikm = 0;

        bool ALL = true;
        while (true)
        {
            ALL = true;
            
            for (int ifile = 0; ifile < filenames.size(); ifile++)
            {
                toKmeans.clear();
                if (ikm >= personActionfeatures[ifile].size())
                {
                    ALL *= true;
                    continue;
                }
                ALL *= false;

                int itf_limit = (ikm + 35 < personActionfeatures[ifile].size()) ? ikm + 35: personActionfeatures[ifile].size(); 
                for(int itf = ikm; itf < itf_limit; itf++)
                {
                    toKmeans.push_back(personActionfeatures[ifile][itf]);
                }
                std::vector<int> result; 

                BOW.setFeatures(toKmeans);
                BOW.getHistogram(result);  
                std::cout<<"Class>> "<<ifile<<std::endl;
                std::cout<<"#Features>> "<<result.size()<<std::endl;
                for(int ires = 0; ires < result.size(); ires++)
                    std::cout << result[ires] << " ";
                

                fhandlerML.AppendLine(result, ifile);  
                fhandlerML.Release(); 
            }

            if (ALL == true)
                break;

            //std::cout << "\nClass = "<< itos[ifile] << " => ";
            //fhandler.AppendLine(result, ifile);
            
            ikm += 35;
        }
        */
    } 
    
    return 0;
}