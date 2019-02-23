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

void getCentroid(std::vector<std::vector<std::vector<float>>> personActionfeatures,
                 std::vector<std::vector<std::vector<float>>>& cuboidsCenters,
                 int classes,
                 int mclusters,
                 int setRandomCenter)
{

    std::cout << "\nKmeans running ...\n\n\tCuboids size = " << cuboidsCenters.size() << std::endl;
    for (int icuboid = 0; icuboid < cuboidsCenters.size(); icuboid++)
    {
        std::cout << "\n\tcuboid = " << icuboid << "\n"<<std::endl;
        kmeans BOW(cuboidsCenters[icuboid], mclusters);

        std::vector<std::vector<float>> features;
        for (int ikm = 0; ikm < personActionfeatures.size(); ikm++)
        {
            features.push_back(personActionfeatures[ikm][icuboid]);
        }

        BOW.setFeatures(features);
        if (setRandomCenter == 0)
            BOW.startingCenters();
        BOW.runKmeans();

        cuboidsCenters[icuboid] = BOW.getCentroids();
    }

}

std::vector<std::vector<std::vector<float>>> runBOW(std::vector<option> database, 
                                                    int mClusters)
{   

    std::vector<std::string> filenames(6);
    std::vector<std::vector<std::vector<float>>> personActionfeatures;
    std::vector<int> personActionfeaturesSize(6,0);
    cv::VideoCapture cap;

    int cuboidSize;
    std::string PATH_DATA               = "../data/";

    
    
    std::vector<std::vector<std::vector<float>>> cuboidsCenters(35);
    //std::string MODEL_CENTROIDS_PATH = "../models/centroids/";
    //std::vector<std::vector<std::vector<float>>> cuboidsCenters;
    //std::string CENTROIDS_FILE_TEMP = "../models/centroids/centroids_person\rperson17.txt";
    //LoadCentroidsFromFile(CENTROIDS_FILE_TEMP, cuboidsCenters);
    std::cout << "data size : " << database.size() << std::endl;

    for (int itrain = 0; itrain < database.size(); itrain+=24)
    {
        for (int i = 0; i < personActionfeatures.size(); i++)
        {
            for (int j = 0; j < personActionfeatures.size(); j++)
                personActionfeatures[i][j].clear();
            personActionfeatures[i].clear();
        }
        personActionfeatures.clear();
     
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

            for (int iv = 0;  iv < database[itrain + idata].sequence.size(); iv+=2)
            //for (int iv = 0;  iv < 2; iv+=2)
            {
                std::pair<int,int> input_sequence(database[itrain + idata].sequence[iv], database[itrain + idata].sequence[iv + 1]);
                //std::pair<int,int> input_sequence(database[itrain + idata].sequence[iv], database[itrain + idata].sequence[iv] + 15);
                OFCM Haralick(108,144);
                std::vector<std::vector<std::vector<float>>> res = Haralick.get_features(cap, input_sequence, cuboidSize);                
                std::cout << "\tseq["<<database[itrain + idata].sequence[iv]<<"-"<<database[itrain + idata].sequence[iv + 1]<<"]\tres.size() = " << res.size() << " x " << res[0].size() << " x " << res[0][0].size()<< ", cuboidSize = "<< cuboidSize << std::endl;


                for(int ir = 0; ir < res.size(); ir++)
                {
                    personActionfeatures.push_back(res[ir]);
                    
                }
                personActionfeaturesSize[ifile]+=res.size();
                //personActionfeaturesSize.push_back()
                for (int ires = 0; ires < res.size(); ires++)
                {
                    for (int jres = 0; jres < res[ires].size(); jres++)
                        res[ires][jres].clear();
                }
            }
            std::cout << std::endl;
            for(int ir = 0; ir < personActionfeaturesSize.size(); ir++)
            {
                std::cout<< "\tpersonActionfeatures[class = "<< ir <<"].size() = " << personActionfeaturesSize[ir] << std::endl;
            }
        }
        std::string CENTROIDS_FILE          = "../models/centroids/centroids_" + database[itrain].person+".txt";
        getCentroid(personActionfeatures, cuboidsCenters, 6, mClusters, itrain);
        SaveCentroidsInFile<float>(CENTROIDS_FILE, cuboidsCenters);
    }

    for (int i = 0; i < personActionfeatures.size(); i++)
    {
        for (int j = 0; j < personActionfeatures.size(); j++)
            personActionfeatures[i][j].clear();
        personActionfeatures[i].clear();
    }
    personActionfeatures.clear();
    
    return cuboidsCenters;
}

void getHistograms( std::vector<option> database, 
                    int mClusters,
                    std::vector<std::vector<std::vector<float>>> cuboidsCenters,
                    std::vector<std::vector<float>>& histograms,
                    std::vector<int>& labels)
{   

    std::vector<std::string> filenames(6);
    std::vector<std::vector<std::vector<std::vector<float>>>> personActionfeatures(6);
    cv::VideoCapture cap;

    int cuboidSize;
    std::string PATH_DATA               = "../data/";

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

            std::cout << "\nreading : "<<PATH_DATA + videofile << "\n"<< std::endl;
            cap.open(PATH_DATA + videofile);

            if (!cap.isOpened())
            {
                std::cout << "Failed to open camera." << std::endl;
            }

            float centroid_class;

            for (int iv = 0;  iv < database[itrain + idata].sequence.size(); iv+=2)
            {
                std::pair<int,int> input_sequence(database[itrain + idata].sequence[iv], database[itrain + idata].sequence[iv + 1]);
                OFCM Haralick(108,144);
                std::vector<std::vector<std::vector<float>>> res = Haralick.get_features(cap, input_sequence, cuboidSize);                
                std::cout << "\tseq["<<database[itrain + idata].sequence[iv]<<"-"<<database[itrain + idata].sequence[iv + 1]<<"]\tres.size() = " << res.size() << " x " << res[0].size() << " x " << res[0][0].size()<< ", cuboidSize = "<< cuboidSize << std::endl;

                for(int ires = 0; ires < res.size(); ires++)
                {
                    std::vector<float> histogram;
                    for(int icuboid = 0; icuboid < res[0].size(); icuboid++)
                    {
                        kmeans BOW(cuboidsCenters[icuboid], mClusters);
                        std::vector<float> single_feature = res[ires][icuboid];
                        centroid_class = (float)BOW.getGoodCluster(single_feature);
                        histogram.push_back(centroid_class);
                    }

                    histograms.push_back(histogram);
                    labels.push_back(ifile);
                }
            }
        }
    }
}