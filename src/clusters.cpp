//#include "headers/utils.hpp"

#include "headers/ofcm.hpp"
#include "headers/kmeans.hpp"
#include "headers/option.hpp"
#include "headers/miscellanius.hpp"
#include "headers/clusters.hpp"

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
            for (int ifet = 0; ifet < personActionfeatures[ikm][icuboid].size(); ifet++)
                features.push_back(personActionfeatures[ikm][icuboid][ifet]);
        }

        BOW.setFeatures(features);
        if (setRandomCenter == 0)
            BOW.startingCenters();

        BOW.runKmeans();

        cuboidsCenters[icuboid] = BOW.getCentroids();
    }

}

std::vector<std::vector<std::vector<float>>> getCuboidCentroids(std::vector<option> database, 

                                                    int mClusters)
{

    std::vector<std::string> filenames(6);
    std::vector<std::vector<std::vector<std::vector<float>>>> personActionfeatures;
    std::vector<int> personActionfeaturesSize(6,0);
    cv::VideoCapture cap;

    int cuboidSize;
    std::string PATH_DATA               = "../data/";



    std::vector<std::vector<std::vector<float>>> cuboidsCenters(35);
    //std::string MODEL_CENTROIDS_PATH = "../models/centroids/";
    //std::vector<std::vector<std::vector<float>>> cuboidsCenters;
    //std::string CENTROIDS_FILE_TEMP = "../models/centroids/centroids_\rperson17.txt";
    //LoadCentroidsFromFile(CENTROIDS_FILE_TEMP, cuboidsCenters);
    std::cout << "data size : " << database.size() << std::endl;

    for (int itrain = 0; itrain < database.size(); itrain+=24)
    {
        for (int i = 0; i < personActionfeatures.size(); i++)
        {
            for (int j = 0; j < personActionfeatures[i].size(); j++)
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
                std::cout << "\tseq["<<database[itrain + idata].sequence[iv]<<"-"<<database[itrain + idata].sequence[iv + 1]<<"]";
                std::vector<std::vector<std::vector<std::vector<float>>>> res = Haralick.get_features(cap, input_sequence, cuboidSize);
                std::cout <<"\tres.size() = " << res.size() << " x " << res[0].size() << " x " << res[0][0].size()<< ", cuboidSize = "<< cuboidSize << std::endl;


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

void clustering( std::vector<option> database, 
                    int mClusters,
                    std::vector<std::vector<std::vector<float>>> cuboidsCenters,
                    std::vector<std::vector<float>>& cuboidsClusters,
                    std::vector<int>& labels)
{
    cv::VideoCapture cap;

    int cuboidSize;
    std::string PATH_DATA               = "../data/";

    std::string MODEL_CENTROIDS_PATH = "../models/centroids/";
    std::cout << "data size : " << database.size() << std::endl;

    for (int itrain = 0; itrain < database.size(); itrain+=24)
    {

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
                std::cout << "Failed to open video." << std::endl;
            }

            float centroid_class;

            for (int iv = 0;  iv < database[itrain + idata].sequence.size(); iv+=2)
            {
                std::pair<int,int> input_sequence(database[itrain + idata].sequence[iv], database[itrain + idata].sequence[iv + 1]);
                OFCM Haralick(108,144);

                std::cout << "\tseq["<<database[itrain + idata].sequence[iv]<<"-"<<database[itrain + idata].sequence[iv + 1]<<"]";
                std::vector<std::vector<std::vector<std::vector<float>>>> res = Haralick.get_features(cap, input_sequence, cuboidSize);                
                std::cout << "\tres.size() = " << res.size() << " x " << res[0].size() << " x " << res[0][0].size()<< ", cuboidSize = "<< cuboidSize << std::endl;

                for(int ires = 0; ires < res.size(); ires++)
                {
                    std::vector<float> clusters;
                    for(int icuboid = 0; icuboid < res[ires].size(); icuboid++)
                    {
                        kmeans BOW(cuboidsCenters[icuboid], mClusters);
                        std::vector<std::vector<float>> setof_feature = res[ires][icuboid];
                        BOW.setFeatures(setof_feature);

                        std::vector<int> histogram;
                        BOW.getHistogram(histogram);

                        for (int hi = 0; hi < histogram.size(); hi++)
                            clusters.push_back((float)histogram[hi]);
                        //centroid_class = (float)BOW.getGoodCluster(single_feature);
                        //clusters.push_back(centroid_class);
                    }

                    cuboidsClusters.push_back(clusters);
                    labels.push_back(ifile);
                }
                for (int i = 0; i < res.size(); i++)
                {
                    for (int j = 0; j < res[i].size(); j++)
                    {
                        res[i][j].clear();
                    }
                    res[i].clear();
                }        
                res.clear();
                
            }
        }
        std::string DATA_FILE       = "../models/test/testdata_until_"+std::to_string(itrain)+".txt";
        std::string LABEL_FILE      = "../models/test/testlabel_until_"+std::to_string(itrain)+".txt";
        FileHandlerML <float, float> fhandlerML(DATA_FILE, LABEL_FILE);

        for(int i = 0; i < cuboidsClusters.size(); i++)
            fhandlerML.AppendRow(cuboidsClusters[i], labels[i]);

        fhandlerML.Release();
    }
}

void saveMeanCentroid(std::vector<option> database, std::vector<std::vector<std::vector<float>>> meanCuboidsCenters)
{
    for (int icub = 0; icub < meanCuboidsCenters.size(); icub++)
    {
        for (int iperson = 0; iperson < database.size()-24; iperson+=24)
        {
            std::string person = database[iperson].person;
            std::string CENTROIDS_FILE          = "../models/centroids/clusters_5/centroids_" + person.substr(1)+".txt";
            std::vector<std::vector<std::vector<float>>> cuboidsCenters;
            LoadCentroidsFromFile(CENTROIDS_FILE, cuboidsCenters);

            for (int icluster = 0; icluster < meanCuboidsCenters[icub].size(); icluster++)
            {
                for (int ifeature = 0; ifeature < meanCuboidsCenters[icub][icluster].size(); ifeature++)
                {
                    meanCuboidsCenters[icub][icluster][ifeature]+= (cuboidsCenters[icub][icluster][ifeature]/7.0);
                }
            }
        }
    }
    std::string CENTROIDS_FILE = "../models/centroids/meanCentroids.txt";
    SaveCentroidsInFile<float>(CENTROIDS_FILE, meanCuboidsCenters);
}
