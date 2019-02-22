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

void getCentroid(std::vector<std::vector<std::vector<std::vector<float>>>> personActionfeatures,
                 std::vector<std::vector<std::vector<float>>>& cuboidsCenters,
                 int classes,
                 int mclusters,
                 int INIT)
{
    
    std::cout << "cuboids size = " << cuboidsCenters.size() << std::endl;
    for (int icuboid = 0; icuboid < cuboidsCenters.size(); icuboid++)
    {
        int ikm = 0;
        bool ALL = true;

        kmeans BOW(cuboidsCenters[icuboid], mclusters);
        bool INIT = true;

        std::vector<std::vector<float>> features;

        while (true)
        {
            ALL = true;
            
            for (int iclass = 0; iclass < classes; iclass++)
            {
                if (ikm >= personActionfeatures[iclass].size())
                {
                    ALL *= true;
                    continue;
                }

                ALL *= false;
                features.push_back(personActionfeatures[iclass][ikm][icuboid]);
            }

            if (ALL == true)
                break;               
            ikm++;
        }


        BOW.setFeatures(features);
        if (INIT == 0)
            BOW.startingCenters(); 
        BOW.runKmeans();       
        
        cuboidsCenters[icuboid] = BOW.getCentroids();
        
        std::cout << "\nCenters size = " << cuboidsCenters[icuboid].size() << " , cubid = " << icuboid << std::endl;
        std::cout << "==================================================================" << std::endl;
        for (int k = 0; k < mclusters; k++)
        {
            for (int icen = 0; icen < cuboidsCenters[icuboid][k].size(); icen++)
            {
                std::cout << cuboidsCenters[icuboid][k][icen] << " ";
                if ((icen+1) % 24 == 0)
                    std::cout << std::endl;
            }
            std::cout << std::endl;
        }     

        cuboidsCenters[icuboid] = BOW.getCentroids();
        
    }

}
std::vector<std::vector<std::vector<float>>> getCentroids(std::vector<option> database, int mClusters){   
    std::vector<std::string> filenames(6);
    std::vector<std::vector<std::vector<std::vector<float>>>> personActionfeatures(6);
    cv::VideoCapture cap;

    int cuboidSize;
    std::string PATH_DATA               = "../data/";

    std::string MODEL_CENTROIDS_PATH = "../models/centroids/";
    std::vector<std::vector<std::vector<float>>> cuboidsCenters(35);
    
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

            std::cout << "\nreading : "<<PATH_DATA + videofile << "\n"<< std::endl;
            cap.open(PATH_DATA + videofile);

            if (!cap.isOpened())
            {
                std::cout << "Failed to open camera." << std::endl;
            }

            //for (int iv = 0;  iv < train_data[itrain + idata].sequence.size(); iv+=2)
            for (int iv = 0;  iv < 2; iv+=2)
            {
                std::pair<int,int> input_sequence(database[itrain + idata].sequence[iv], database[itrain + idata].sequence[iv + 1]);
                OFCM Haralick(108,144);
                std::vector<std::vector<std::vector<float>>> res = Haralick.get_features(cap, input_sequence, cuboidSize);
                
                std::cout << "\tres.size() = " << res.size() << " x " << res[0].size() << " x " << res[0][0].size()<< ", cuboidSize = "<< cuboidSize << std::endl;

                for(int ir = 0; ir < res.size(); ir++)
                {
                    personActionfeatures[ifile].push_back(res[ir]);
                }
                for (int ires = 0; ires < res.size(); ires++)
                {
                    for (int jres = 0; jres < res[ires].size(); jres++)
                        res[ires][jres].clear();
                }
            }
            for(int ir = 0; ir < personActionfeatures.size(); ir++)
            {
                std::cout<< "\tpersonActionfeatures["<< ir <<"].size() = " << personActionfeatures[ir].size() << std::endl;
            }
        }
        getCentroid(personActionfeatures, cuboidsCenters, 6, mClusters, itrain);
    }

    /*
    * All data on personActionfeatures
    */
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
    //}

    return cuboidsCenters;
}


int getHistograms(std::vector<std::vector<float>> centers, int K_CLASES){   

    //std::vector<int> trainingPerson    = {11, 12, 13, 14, 15, 16, 17, 18};
    std::vector<std::string> filenames(6);
    std::vector<std::vector<std::vector<float>>> personActionfeatures(6);
    cv::VideoCapture cap;
    bool INIT          = true;
    int cuboidSize;
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
                std::vector<std::vector<std::vector<float>>> res = Haralick.get_features(cap, input_sequence, cuboidSize);

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